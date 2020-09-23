from version import create_versions
from name_utils import process_name, process_array_size, process_enum_name
from enums import get_enums
from bitfields import get_bitfields
from compounds import get_compounds
from objects import get_module_names, get_module
from expression import Expression
from generator_context import GeneratorContext, get_basic_types, get_pointer_types, get_string_types
from typedefs import get_typedefs
from replace_tokens import replace_tokens
from factory import FactoryMap

import os
import xml.dom
import xml.dom.minidom
import re
import sys
import string
from enum import Enum


if __name__ == '__main__':

    path = 'nif.xml'
    if os.path.exists(path):

        document = xml.dom.minidom.parse(path)
    else:
        raise ImportError("nif.xml not found")

    # Replace all tokens in attributes with values from <token> elements.
    document = replace_tokens(document)

    # Get a version object for each engine version for a game.
    versions = create_versions(document, 'Fallout NV')

    for version in versions:
        if not os.path.exists(version.id):
            os.mkdir(version.id)

        # Create a context for this version's code.
        context = GeneratorContext(version, get_basic_types(), get_string_types(), get_pointer_types())

        # Write the typedefs file.
        typedefs = get_typedefs()
        with open(str(version.id) + '/typedefs.hpp', 'w', encoding='utf-8') as f:
            f.write(typedefs)

        # Parse and register the enum types.
        # Write the enums to a file.
        enums = get_enums(context, document, version, 'NiEnums')
        with open(str(version.id) + '/enums.hpp', 'w', encoding='utf-8') as f:
            f.write(enums)

        # Parse and register the bitfield types.
        # Write the bitfields to a file.
        bitfields = get_bitfields(context, document, version)
        with open(str(version.id) + '/bitfields.hpp', 'w', encoding='utf-8') as f:
            f.write(bitfields)

        # Parse and register the compound types.
        # Write the compound headers to a file.
        compounds_hpp, compounds_cpp = get_compounds(context, document, version)
        with open(str(version.id) + '/structs.hpp', 'w', encoding='utf-8') as f:
            f.write(compounds_hpp)

        # Write the compounds compilaton unit.
        with open(str(version.id) + '/structs.cpp', 'w', encoding='utf-8') as f:
            f.write(compounds_cpp)

        # Get the names of all available modules.
        module_names = get_module_names(document)

        factory = FactoryMap()

        for module_name in module_names:

            # Parse the object types in the module.
            module = get_module(context, document, version, module_name)
            if not os.path.exists(str(version.id) + '/' + module_name):
                os.mkdir(str(version.id) + '/' + module_name)

            # Register the module for inclusion.
            factory.append_module(module_name)
            module_header = ''

            for name, hpp, cpp in module.get_module_codes():

                # Register factory method.
                factory.append_mapping(name)
                
                # Register object in module-level include file.
                module_header += '#include "' + name + '"\n'

                # Write the header and compilation unit code of the object.
                with open(str(version.id) + '/' + module_name + '/' + name + '.hpp', 'w', encoding='utf-8') as f:
                    f.write(hpp)
                with open(str(version.id) + '/' + module_name + '/' + name + '.cpp', 'w', encoding='utf-8') as f:
                    f.write(cpp)

            # Write the module-level include file.
            with open(str(version.id) + '/' + module_name + '/' + module_name + '.hpp', 'w', encoding='utf-8') as f:
                f.write(module_header)

        # Write the map of factory methods.
        with open(str(version.id) + '/factory.hpp', 'w', encoding='utf-8') as f:
            f.write(factory.get_hpp())
        with open(str(version.id) + '/factory.cpp', 'w', encoding='utf-8') as f:
            f.write(factory.get_cpp())