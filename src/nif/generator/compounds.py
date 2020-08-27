from field_types import FieldCode
from name_utils import process_name

class CompoundCode:
    """
        Represents the C++ code for a compound.
    """

    def __init__(self, context, element):
        self.name = element.getAttribute('name')
        context.add_compound_type(self.name)
        if element.firstChild is not None:
            self.description = element.firstChild.nodeValue.strip()
        else:
            self.description = None

        fieldElements = element.getElementsByTagName('field')
        
        if element.hasAttribute('generic') and element.getAttribute('generic') == 'true':
            self.generic = True
        else:
            self.generic = False

        self.fields = list()
        for el in fieldElements:
            if context.is_correct_version(el):
                self.fields.append(FieldCode(context, el))

    def _get_declaration(self):
        """
            Gets the declaration code.
        """
        if self.generic:
            out = 'template <typename T>\n'
        else:
            out = ''

        out += 'struct ' + self.name + ' {\n'

        for field in self.fields:
            out += field.get_declaration()

        out += '\n\n\t'+self.name+'(NifReader& reader);\n'
        out += '\n\n\t~'+self.name+'();\n'
        out += '\tvoid resolvePointers(NifData& data);\n'
        out += '};\n'
        return out

    def _get_constructor(self):
        """
            Gets the constructor code.
        """
        if self.generic:
            out = 'template <typename T>\n ' + self.name + '<T>::' + self.name + '(NifReader& reader) {\n'
        else:
            out = self.name + '::' + self.name + '(NifReader& reader) {\n'
        for field in self.fields:
            out += field.get_constructor_code()
        out += '}\n'
        return out

    def _get_destructor(self):
        """
            Gets the destructor code.
        """
        if self.generic:
            out = 'template <typename T>\n ' + self.name + '<T>::~' + self.name + '() {\n'
        else:
            out = self.name + '::~' + self.name + '() {\n'
        for field in self.fields:
            out += field.get_destructor_code()
        out += '}\n'
        return out

    def _get_resolve_pointers(self):
        """
            Gets the resolve pointers code.
        """
        if self.generic:
            out = 'template <typename T>\nvoid ' + self.name + '<T>::resolvePointers(NifData& data) {\n'
        else:
            out = 'void ' + self.name + '::resolvePointers(NifData& data) {\n'
        for field in self.fields:
            out += field.get_resolve_pointers_code()
        out += '}\n'
        return out

    def get_hpp(self):
        """
            Gets the header code.

            Returns:
                str
        """
        return self._get_declaration()
        
    def get_cpp(self):
        """
            Gets the compilation unit code.

            Returns:
                str
        """
        out = self._get_constructor()
        out += self._get_destructor()
        out += self._get_resolve_pointers()
        return out


def get_compounds(context, document, version):
    """
        Gets the header and compilation unit C++ code for compounds from the document
        for the given game version.

        Compounds are represented as structs.
        All compounds are put into a single file.

        Prerequisites:
            All enum and bitfield types must be regstered in the context, as they
            can be compound members.

        Parameters:
            context (GeneratorContext) -- Current context.
            document (xml.Document) -- The document.
            version (str) -- The version.

        Returns:
            structs_hpp (str) -- Header file code.
            structs_cpp (str) -- Compilation unit file code.
    """
    structs_hpp = """
    #pragma once
    #include "typedefs.hpp"
    #include "enums.hpp"
    #include "bitfields.hpp"
    #include <cstdint>
    """
    structs_cpp = """
    #include "structs.hpp"
    """

    for element in document.getElementsByTagName('compound'):
        if element.hasAttribute('versions'):
            if not version.id in element.getAttribute('versions'):
                continue
        if element.hasAttribute('until'):
            if not version.is_before(element.getAttribute('until')):
                continue
        if element.hasAttribute('since'):
            if not version.is_after(element.getAttribute('since')):
                continue

        compound = CompoundCode(context, element)
        structs_hpp += compound.get_hpp()
        structs_cpp += compound.get_cpp()

    return structs_hpp, structs_cpp