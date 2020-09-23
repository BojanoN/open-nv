from name_utils import process_enum_name

class EnumOption:
    """
        Represents an enum option, includes the name and numeric value.
    """

    def __init__(self, element, prefix=None, is_flags=False):
        self.name = process_enum_name(element.getAttribute('name'))
        if prefix is not None:
            self.name = prefix + '_' + self.name
        if is_flags:
            self.value = '0x{:08X}'.format(1 << int(element.getAttribute('bit')))
        else:
            self.value = element.getAttribute('value')

        if element.firstChild is not None:
            self.description = element.firstChild.nodeValue.strip()
        else:
            self.description = ''

class EnumCode:
    """
    Represents the C++ code for an enum.
    Includes the enum name, the enum base and all enum options.
    
    Call get_hpp to receive the enum declaration.
    """

    def __init__(self, context, element, is_flags=False):
        self.name = element.getAttribute('name')
        #print(self.name)
        context.add_enum_type(self.name)
        self.type = context.get_basic_type(element.getAttribute('storage'))
        if self.type is None:
            raise ValueError('Invalid enum base type: ' + str(self.type) + ' is not a basic type!')

        self.description = element.firstChild.nodeValue.strip()
        
        if element.hasAttribute('prefix'):
            self.prefix = element.getAttribute('prefix')
        else:
            self.prefix = None

        self.options = list()
        for option in element.getElementsByTagName('option'):
            self.options.append(EnumOption(option, self.prefix, is_flags))

    def _get_declaration(self):
        """
            Constructs a declaration based on the available options.

            Return:
                (str) -- declaration
        """
        out = 'enum ' + self.name + ' : ' + self.type + ' {\n'

        length = len(self.options)
        for i, option in enumerate(self.options):
            out += '\t' + option.name + ' = ' + option.value
            if i != length - 1:
                out += ',\n'
            else:
                out += '\n'
        out += '};\n'
        return out

    @classmethod
    def get_includes(cls):
        """
            Returns the include guard + necessary include directives for all enums.
        """
        return """
        #pragma once
        #include <cstdint>
        """

    def get_hpp(self):
        """
            Returns the enum declaration.

            Returns:
                (str) -- Enum declaration.
        """
        return self._get_declaration()



def get_enums(context, document, version, namespace):
    """
        Reads enums available in the given version from the document and creates a string representation
        of a C++ header file with enum declarations.
        Enum types are registered with the given generator context.
        
        Bitflags are also considered enums in the generated code.

        All enums are put into a single file.
        All enums are put into the given namespace as to not contaminate the outer namespace.
        The enums are unscoped to facilitate auto-conversion to their base types.

        Parameters:
            context (GeneratorContext) -- Current generator context.
            document (xml.Document) -- The document.
            version (Version) -- The version of the game.

        Returns:
            enum_hpp (str) -- String representation of an enum file.
    """
    enum_hpp = EnumCode.get_includes()

    enum_hpp += 'namespace ' + namespace + ' {\n'
    for element in document.getElementsByTagName('enum'):
        if element.hasAttribute('versions'):
            if not version.id in element.getAttribute('versions'):
                continue

        enum = EnumCode(context, element, is_flags=False)
        enum_hpp += enum.get_hpp()

    for element in document.getElementsByTagName('bitflags'):
        if element.hasAttribute('versions'):
            if not version.id in element.getAttribute('versions'):
                continue

        enum = EnumCode(context, element, is_flags=True)
        enum_hpp += enum.get_hpp()

    enum_hpp += '}; // namespace\n'
    return enum_hpp