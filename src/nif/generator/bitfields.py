from name_utils import process_name

class BitfieldMember:
    """
        A bitfield struct member.

        Includes the name, base type, bit size, bit mask and bit offsets
        of this member.
    """

    def __init__(self, context, element):
        self.name = process_name(element.getAttribute('name'))
        self.type = element.getAttribute('type')
        if context.is_basic_type(self.type):
            self.type = context.get_basic_type(self.type)
        elif context.is_enum_type(self.type):
            self.type = 'NiEnums::' + self.type
        else:
            raise ValueError('Invalid bitfield member: ' + str(self.type) + ' is not a basic or enum type!')

        if element.firstChild is not None:
            self.description = element.firstChild.nodeValue.strip()
        else:
            self.description = ''

        self.width = element.getAttribute('width')
        self.mask = element.getAttribute('mask')
        self.position = element.getAttribute('pos')


        self.declaration = '\t' + self.type + ' ' + self.name + ' : ' + self.width + '; // ' + self.mask + '\n'


    def get_bit_and(self, name, result_name):
        """
            Gets the bitwise and operator overload code which tests the bit 
            values of this member and the function parameter. 

            Parameters:
                name (str) -- Name of the operator& function parameter.
                result_name (str) -- Name of the operator& function return value.

            Returns:
                str -- function code
        """
        return '\t\t' + result_name +' += ((' + self.name +' & (' + name + ' >> ' + self.position + ')) << ' + self.position + ') & ' + self.mask + ';\n'

    def get_assign(self, name):
        """
            Gets the assignment operator overload code which assigns to this member.

            Parameters:
                name (str) -- Name of the operator= function parameter.

            Returns:
                str -- function code
        """
        if 'NiEnums::' in self.type:
            return '\t' + self.name + ' = static_cast<'+self.type+'>((' + name + ' & ' + self.mask + ') >> ' + self.position + ');\n'
        else:
            return '\t' + self.name + ' = (' + name + ' & ' + self.mask + ') >> ' + self.position + ';\n'

class BitfieldCode:
    """
        Represents the C++ code for a structure of bitfields.

        Call get_hpp to get the declaration code.
        The declaration code includes inline function definitions 
        for the assignment (=) and bitwise and (&) operators.
    """

    def __init__(self, context, element):
        self.name = element.getAttribute('name')
        self.description = element.firstChild.nodeValue.strip()
        self.type = context.get_basic_type(element.getAttribute('storage'))
        if self.type is None:
            raise ValueError('Invalid bitfield storage type: ' + element.getAttribute('storage') + ' is not a basic type!')

        context.add_bitfield_type(self.name, self.type)

        self.members = list()
        for member in element.getElementsByTagName('member'):
            self.members.append(BitfieldMember(context, member))


    def _get_declaration(self):
        """
            Gets the declaration and inline function definitions.

            Return:
                str
        """
        out = 'struct ' + self.name + ' {\n'

        bit_and = '\t' + self.type + ' operator&(const ' + self.type + '& b) const {\n'
        bit_and += '\t\t' + self.type + ' result = 0;\n'

        assign = '\t' + self.name + '& operator=(const ' + self.type + '& value) {\n' 

        for member in self.members:
            out += member.declaration
            bit_and += member.get_bit_and('b', 'result')
            assign += member.get_assign('value')

        bit_and += '\t\treturn result;\n\t}\n'
        assign += '\t\treturn *this;\n\t}\n'
        out += '\t' + self.name + '() { }\n'
        out += bit_and
        out += assign
        out += '};\n'
        return out

    @classmethod
    def get_includes(cls):
        """
            Gets the include guard + necessary include directives for all
            bitfield structs.

            Return:
                str
        """
        out = """
        #pragma once
        #include "enums.hpp"
        #include "typedefs.hpp"
        """
        return out

    def get_hpp(self):
        """
            Gets the struct declaration and inline function definitions.

            Return:
                out (str) -- The struct declaration and inline function definitions.
        """
        out = self._get_declaration()
        return out


def get_bitfields(context, document, version):
    """
        Gets all bitfield structs from the document available in this version.
        Types are registered as bitfield types in the generator context.

        Bitfields are represented as structs with overloaded assignment and bitwise and operators.
        All bitfields are put into a single file.
    
        Prerequisites:
            All enum types must be registered in the generated context, since some
            bitfield members are enum types.

        Parameters:
            context (GeneratorContext) -- Current context.
            document (xml.Document) -- The document.
            version (Version) -- The game version.

        Returns:
            bitfields_hpp (str) -- The string representation of a C++ header file
                containing bitfield struct declarations and inline function definitions.
    """
    bitfields_hpp = BitfieldCode.get_includes()

    for element in document.getElementsByTagName('bitfield'):
        if element.hasAttribute('versions'):
            if not version.id in element.getAttribute('versions'):
                continue

        bitfield = BitfieldCode(context, element)
        bitfields_hpp += bitfield.get_hpp()

    return bitfields_hpp