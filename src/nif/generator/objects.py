from field_types import FieldCode, PointerType, CompoundType

class ObjectCode:
    """
       Represents the C++ code for a nif file object. 
    """

    def __init__(self, context, element):
        self.module = element.getAttribute('module')
        self.name = element.getAttribute('name')

        if element.firstChild is not None:
            self.description = element.firstChild.nodeValue.strip()
        else:
            self.description = None
        
        if element.hasAttribute('inherit'):
            self.parent = element.getAttribute('inherit')
        else:
            self.parent = None

        fieldElements = element.getElementsByTagName('field')
        self.fields = list()
        for el in fieldElements:
            if context.is_correct_version(el):
                self.fields.append(FieldCode(context, el))

    def _get_includes(self):
        """
            Gets the include directives for this object.

            Returns:
                str
        """
        out = """
        #pragma once
        #include "typedefs.hpp"
        #include "enums.hpp"
        #include "bitfields.hpp"
        #include "structs.hpp"
        #include <memory>
        #include <vector>
        #include <array>
        #include "../nifreader.hpp"
        #include "../nifpointer.hpp"
        class NifData;
        """
        if self.parent is not None:
            out += 'struct ' + self.parent + ';\n'
            out += '#include "' + self.parent + '.hpp"\n'


        has_vector = False
        for field in self.fields:
             if type(field.type) == PointerType:
                out += 'struct ' + field.type.target_type + ';\n'
                out += '#include "' + field.type.target_type + '.hpp"\n'
             elif type(field.type) == CompoundType:
                if field.type.is_array and not has_vector:
                    out += '#include <vector>\n'
                    has_vector = True

        out += '#include <cstdint>\n'
        out += '\n'

        return out

    def _get_declaration(self):
        """
            Gets the declaration of this object.

            Returns:
                str
        """
        if self.parent is not None:
            out = 'struct ' + self.name + ' : public ' + self.parent + ' {\n'
        else:
            out = 'struct ' + self.name + ' {\n'

        for field in self.fields:
            out += field.get_declaration()

        out += '\n\n\t'+self.name+'(NifReader& reader);\n'
        out += '\n\n\tvirtual ~'+self.name+'();\n'
        out += '\tstatic NiObject* create(NifReader& reader);\n'
        out += '\tvirtual void resolvePointers(NifData& data);\n'
        out += '};\n'
        return out

    def _get_constructor(self):
        """
            Gets the constructor code of this object.

            Returns:
                str
        """
        if self.parent is not None:
            out = self.name + '::' + self.name + '(NifReader& reader) : ' + self.parent + '(reader) {\n'
        else:
            out = self.name + '::' + self.name + '(NifReader& reader) {\n'

        for field in self.fields:
            if field.only_type is None or field.only_type == self.name:
                out += field.get_constructor_code()
        out += '}\n'
        return out

    def _get_destructor(self):
        """
            Gets the destructor code of this object.

            Returns:
                str
        """
        out = self.name + '::~' + self.name + '() {\n'
        for field in self.fields:
            if field.only_type is None or field.only_type == self.name:
                out += field.get_destructor_code()
        out += '}\n'
        return out

    def _get_resolve_pointers(self):
        """
            Gets the pointer resolution code of this object.

            Returns:
                str
        """
        out = 'void ' + self.name + '::resolvePointers(NifData& data) {\n'
        if self.parent is not None:
            out += '\t' + self.parent + '::resolvePointers(data);\n'
        for field in self.fields:
            if field.only_type is None or field.only_type == self.name:
                out += field.get_resolve_pointers_code()
        out += '}\n'
        return out

    def _get_factory(self):
        """
            Gets the object factory method.

            Returns:
                str
        """
        out = 'NiObject* ' + self.name + '::create(NifReader& reader) {\n'
        out += '\treturn new ' + self.name + '(reader);\n'
        out += '}\n'
        return out

    def get_hpp(self):
        """
            Gets the header file code for this object.

            Returns:
                str
        """
        out = self._get_includes()
        out += self._get_declaration()
        return out
        
    def get_cpp(self):
        """
            Gets the compilation unit code for this object.

            Returns:
                str
        """
        out = '#include "' + self.name + '.hpp"\n'
        out += self._get_constructor()
        out += self._get_factory()
        out += self._get_destructor()
        out += self._get_resolve_pointers()
        return out
        


class ObjectModule:
    """
        Represents a module. A module is a collection of objects.
    """

    def __init__(self, context, version, module_elements):
        self.module_hpps = dict()
        self.module_cpps = dict()
            
        for module_element in module_elements:
            if module_element.hasAttribute('versions'):
                if not version.id in module_element.getAttribute('versions'):
                    continue
            if module_element.hasAttribute('until'):
                if not version.is_before(module_element.getAttribute('until')):
                    continue
            if module_element.hasAttribute('since'):
                if not version.is_after(module_element.getAttribute('since')):
                    continue

            name = module_element.getAttribute('name')
            if name == "NiDataStream":
                breakpoint()
            obj = ObjectCode(context, module_element)
            self.module_hpps[name] = obj.get_hpp()
            self.module_cpps[name] = obj.get_cpp()


    def get_module_codes(self):
        """
            Yields the header and compilation unit codes for all objects in this module.

            Yields:
                key (str) -- Object name.
                self.module_hpps[key] -- Object header code.
                self.module_cpps[key] -- Object compilation unit code.
        """
        for key in self.module_hpps.keys():
            yield key, self.module_hpps[key], self.module_cpps[key]



def get_module(context, document, version, module_name):
    """ 
        Extracts the code from the document for the given version for the given module.

        Prerequisites:
            All enum, bitfield and compound types must be registered in the context.

        Parameters:
            context (GeneratorContext) -- Current context.
            document (xml.Document) -- The document.
            version (Version) -- The version.
            module_name (str) -- Name of the module.

        Returns:
            module (ModuleCode) -- object with code for the module.
    """
    module = ObjectModule(context, version, [element for element in document.getElementsByTagName('niobject') if element.getAttribute('module') == module_name])
    return module


def get_module_names(document):
    """
        Gets all module names from this document.

        Parameters:
            document (xml.Document) -- The document.
            
        Returns:
            modules (list) -- List of module names.
    """
    modules = list()
    for module in document.getElementsByTagName('module'):
        name = module.getAttribute('name')
        #modules[name] = includes_header
        #if module.hasAttribute('depends'):
        #    for dependency in module.getAttribute('depends').split(' '):
        #        modules[name] += '#include "' + dependency + '.hpp"\n'
        modules.append(module.getAttribute('name'))
    return modules