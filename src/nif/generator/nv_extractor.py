import os
import xml.dom
import xml.dom.minidom
import re
import sys
import string
from enum import Enum

class Version:

    version_map = dict()

    def __init__(self, v_id, num, user_num, bs_num):
        self.id = v_id
        self.num = self.get_value(num)
        self.user_num = user_num
        self.bs_num = bs_num

        self.Version = self.num
        self.User_Version = int(self.user_num.split(' ')[0])
        self.BS_Header_BS_Version = int(self.bs_num.split(' ')[0])

    def __str__(self):
        return self.id + '0x{:08X}'.format(self.num)

    @classmethod
    def add_version_to_map(cls, ver_id, num):
        cls.version_map[ver_id] = cls.get_value(num)

    @staticmethod
    def get_value(string_value):
        int_value = 0
        parts = [int(value) for value in string_value.split(sep='.')]
        for part, offset in zip(parts, [24, 16, 8, 0]):
            int_value += part << offset
        return int_value

    def is_before(self, ver_id):
        return self.num <= self.version_map[ver_id]

    def is_after(self, ver_id):
        return self.num >= self.version_map[ver_id]


def create_versions(document, game_name):
    versions = list()
    for element in document.getElementsByTagName('version'):
        description = element.firstChild.nodeValue.strip()
        Version.add_version_to_map(element.getAttribute('id'), element.getAttribute('num'))
        if game_name in description:
            versions.append(Version(element.getAttribute('id'), element.getAttribute('num'), element.getAttribute('user'), element.getAttribute('bsver')))
    return versions

class VersionList:

    def __init__(self, xml, game_name):
        self.version_ids = list()
        self._all_versions = dict()
        first = True
        for element in xml.getElementsByTagName('version'):
            description = element.firstChild.nodeValue.strip()
            self._all_versions[element.getAttribute('id')] = element.getAttribute('num')
            if game_name in description:
                self.version_ids.append(element.getAttribute('id'))
                if first:
                    self.version_val = self.get_value(element.getAttribute('num'))
                    first = False

    @staticmethod
    def get_value(string_value):
        int_value = 0
        parts = [int(value) for value in string_value.split(sep='.')]
        for part, offset in zip(parts, [24, 16, 8, 0]):
            int_value += part << offset
        return int_value
    
    def __str__(self):
        output = ''
        for ver_id in self.version_ids:
            output += ver_id + '\n'
        output += '0x{:08X}'.format(self.version_val)
        return output

    def get_number(self, ver_id):
        return self.get_value(self._all_versions[ver_id])

    def __getitem__(self, key):
        return self.version_ids[key]

    def is_before(self, ver_id):
        return self.version_val <= self.get_value(self._all_versions[ver_id])

    def is_after(self, ver_id):
        return self.version_val >= self.get_value(self._all_versions[ver_id])

class Expression:

    operators = ['==', '!=', '>=', '<=', '&&', '||', '&', '|', '-', '+', '>', '<', '/', '*']

    def __init__(self, expr_str, name_filter=None):
        #if expr_str == '!(BS_Header_BS_Version <= 16)':
        #    breakpoint()
        self._code = expr_str
        left, self._op, right = self._partition(expr_str)
        self._left = self._parse(left, name_filter)
        if right:
            self._right = self._parse(right, name_filter)
        else:
            self._right = ''

    @classmethod
    def _parse(cls, expr_str, name_filter=None): # type: (str, Callable[[str], str]) -> str
        """Returns an Expression, string, or int, depending on the
        contents of <expr_str>."""
        # brackets or operators => expression
        if ("(" in expr_str) or (")" in expr_str):
            return Expression(expr_str, name_filter)
        for op in cls.operators:
            if expr_str.find(op) != -1:
                return Expression(expr_str, name_filter)

        mver = re.compile("[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+")
        iver = re.compile("[0-9]+")
        # try to convert it to an integer
        try:
            if mver.match(expr_str):
                return "0x%08X"%(VersionList.get_value(expr_str))
            elif iver.match(expr_str):
                return str(int(expr_str))
        except ValueError:
            pass
        # failed, so return the string, passed through the name filter
        return name_filter(expr_str) if name_filter else expr_str

    @staticmethod
    def _scanBrackets(expr_str, fromIndex=0):
        """Looks for matching brackets.

        >>> Expression._scanBrackets('abcde')
        (-1, -1)
        >>> Expression._scanBrackets('()')
        (0, 1)
        >>> Expression._scanBrackets('(abc(def))g')
        (0, 9)
        >>> s = '  (abc(dd efy 442))xxg'
        >>> startpos, endpos = Expression._scanBrackets(s)
        >>> print s[startpos+1:endpos]
        abc(dd efy 442)
        """
        startpos = -1
        endpos = -1
        scandepth = 0
        for scanpos in range(fromIndex, len(expr_str)):
            scanchar = expr_str[scanpos]
            if scanchar == "(":
                if startpos == -1:
                    startpos = scanpos
                scandepth += 1
            elif scanchar == ")":
                scandepth -= 1
                if scandepth == 0:
                    endpos = scanpos
                    break
        else:
            if startpos != -1 or endpos != -1:
                raise ValueError("expression syntax error (non-matching brackets?)")
        return (startpos, endpos)

    @classmethod
    def _partition(cls, expr_str):
        """Partitions expr_str. See examples below.

        >>> Expression._partition('abc || efg')
        ('abc', '||', 'efg')
        >>> Expression._partition('abc||efg')
        ('abc', '||', 'efg')
        >>> Expression._partition('abcdefg')
        ('abcdefg', '', '')
        >>> Expression._partition(' abcdefg ')
        ('abcdefg', '', '')
        >>> Expression._partition(' (a | b) & c ')
        ('a | b', '&', 'c')
        >>> Expression._partition('(a | b)!=(b&c)')
        ('a | b', '!=', 'b&c')
        >>> Expression._partition('(a== b) &&(( b!=c)||d )')
        ('a== b', '&&', '( b!=c)||d')
        """
        # check for unary operators
        if expr_str.strip().startswith('!'):
            return expr_str.lstrip(' !'), '!', None
        lenstr = len(expr_str)
        # check if the left hand side starts with brackets
        # and if so, find the position of the starting bracket and the ending
        # bracket
        left_startpos, left_endpos = cls._scanBrackets(expr_str)
        if left_startpos >= 0:
            # yes, it is a bracketted expression
            # so remove brackets and whitespace,
            # and let that be the left hand side
            left_str = expr_str[left_startpos+1:left_endpos].strip()

            # the next token should be the operator
            # find the position where the operator should start
            op_startpos = left_endpos+1
            while op_startpos < lenstr and expr_str[op_startpos] == " ":
                op_startpos += 1
            if op_startpos < lenstr:
                # to avoid confusion between && and &, and || and |,
                # let's first scan for operators of two characters
                # and then for operators of one character
                for op_endpos in range(op_startpos+1, op_startpos-1, -1):
                    op_str = expr_str[op_startpos:op_endpos+1]
                    if op_str in cls.operators:
                        break
                else:
                    breakpoint()
                    raise ValueError("expression syntax error: expected operator at '%s'"%expr_str[op_startpos:])
            else:
                return cls._partition(left_str)
        else:
            # it's not... so we need to scan for the first operator
            for op_startpos, ch in enumerate(expr_str):
                if ch == ' ':
                    continue
                if ch == '(' or ch == ')':
                    breakpoint()
                    raise ValueError("expression syntax error: expected operator before '%s'"%expr_str[op_startpos:])
                # to avoid confusion between && and &, and || and |,
                # let's first scan for operators of two characters
                for op_endpos in range(op_startpos+1, op_startpos-1, -1):
                    op_str = expr_str[op_startpos:op_endpos+1]
                    if op_str in cls.operators:
                        break
                else:
                    continue
                break
            else:
                # no operator found, so we are done
                left_str = expr_str.strip()
                op_str = ''
                right_str = ''
                return left_str, op_str, right_str
            # operator found! now get the left hand side
            left_str = expr_str[:op_startpos].strip()

        return left_str, op_str, expr_str[op_endpos+1:].strip()

    def __str__(self): # type: () -> str
        """Reconstruct the expression to a string."""
        if type(self._left) == str:
            if not self._left.isdigit():
                self._left = '_'.join(self._left.split(sep=' '))
        left = str(self._left)
        if not self._op:
            return left
        if type(self._right) == str:
            if not self._right.isdigit():
                self._right = '_'.join(self._right.split(sep=' '))
        right = str(self._right)
        return left + ' ' + self._op + ' ' + right
    
    def eval(self, data=None):
        """Evaluate the expression to an integer."""

        if isinstance(self._left, Expression):
            left = self._left.eval(data)
        elif isinstance(self._left, str) and self._left.isdecimal():
            left = int(self._left)
        elif isinstance(self._left, str) and all(c in string.hexdigits for c in self._left[2:]):
            left = int(self._left[2:], 16)
        elif isinstance(self._left, str):
            left = getattr(data, self._left) if self._left != '""' else ""
        elif isinstance(self._left, str):
            left = int(self._left)
        else:
            assert isinstance(self._left, int) # debug
            left = self._left

        if not self._op:
            return left

        try:
            if self._right == '':
                right = None
            elif isinstance(self._right, Expression):
                right = self._right.eval(data)
            elif isinstance(self._right, str) and self._right.isdecimal():
                right = int(self._right)
            elif isinstance(self._right, str) and all(c in string.hexdigits for c in self._right[2:]):
                right = int(self._right[2:], 16)
            elif isinstance(self._right, str):
                right = getattr(data, self._right) if self._right != '""' else ""
            elif isinstance(self._right, str):
                right = int(self._right)
            else:
                assert isinstance(self._right, int) # debug
                right = self._right
        except ValueError:
            breakpoint()

        if self._op == '==':
            return int(left == right)
        elif self._op == '!=':
            return int(left != right)
        elif self._op == '>=':
            return int(left >= right)
        elif self._op == '<=':
            return int(left <= right)
        elif self._op == '>':
            return int(left > right)
        elif self._op == '<':
            return int(left < right)
        elif self._op == '&&':
            return int(left and right)
        elif self._op == '||':
            return int(left or right)
        elif self._op == '&':
            return left & right
        elif self._op == '|':
            return left | right
        elif self._op == '-':
            return left - right
        elif self._op == '+':
            return left + right
        elif self._op == '/':
            return left / right
        elif self._op == '*':
            return left * right
        elif self._op == '!':
            return not left
        else:
            raise NotImplementedError("expression syntax error: operator '" + self._op + "' not implemented")



def replace_tokens(xml):
    tokens = xml.getElementsByTagName('token')
    for token in tokens:
        replacer_name = token.getAttribute('name')
        targets = token.getAttribute('attrs').split()
        replacements = xml.getElementsByTagName(replacer_name)
        replacement_map = dict()
        for replacement in replacements:
            replacement_map[replacement.getAttribute('token')] = replacement.getAttribute('string')

        fields = xml.getElementsByTagName('field')
        fields.extend(document.getElementsByTagName('compound'))
        fields.extend(document.getElementsByTagName('niobject'))
        fields.extend(document.getElementsByTagName('enum'))
        fields.extend(document.getElementsByTagName('bitfield'))
        fields.extend(document.getElementsByTagName('bitflags'))
        fields.extend(document.getElementsByTagName('enum'))
        for field in fields:
            for target in targets:
                if field.hasAttribute(target):
                    value = field.getAttribute(target)
                    for old, new in replacement_map.items():
                        value = value.replace(old, new)
                    field.setAttribute(target, value)
    return xml

basic_types = {
    'char' : 'int8_t',
    'byte' : 'uint8_t',
    'short' : 'int16_t',
    'ushort' : 'uint16_t',
    'int' : 'int32_t',
    'uint' : 'uint32_t',
    'ulittle32' : 'uint32_t',
    'int64' : 'int64_t',
    'uint64' : 'uint64_t',
    'float' : 'float',
    'hfloat' : 'float',
    'bool' : 'nif_bool_t',
    'BlockTypeIndex' : 'int16_t',
    'FileVersion' : 'uint32_t',
    'Ptr' : 'nif_ptr_t',
    'Ref' : 'nif_ref_t',
    'StringOffset' : 'uint32_t',
    'NiFixedString' : 'uint32_t',
    'HeaderString' : 'std::string',
    'LineString' : 'std::string'
}


ptr_types = ['Ptr', 'Ref', 'nif_ptr_t', 'nif_ref_t']


string_format_ids = {
    'uint8_t' : '%u',
    'uint16_t' : '%u',
    'uint32_t' : '%u',
    'uint64_t' : '%u',
    'int8_t' : '%c',
    'int16_t' : '%d',
    'int32_t' : '%d',
    'int64_t' : '%d',
    'float' : '%f',
    'hfloat' : '%f',
    'nif_ref_t' : 'Ref: %u',
    'nif_ptr_t' : 'Ptr: %u',
    'std::string' : '%s',
    'SizedString' : '%s',
    'SizedString16' : '%s',
    'ExportString' : '%s',
    'nif_bool_t' : 'Bool: %d'
}

def process_name(name):
    if ':' in name:
        name = name.replace(':', '')
    if name == '#ARG#':
        return '0'
    parts = name.split(' ')
    parts[0] = parts[0].lower()
    return ''.join(parts)

def process_array_size(array_size):
    #if array_size == 'Num Pixels * Num Faces':
    #    breakpoint()
    for op in ['*', '+', '&']:
        if op in array_size:
            result = ''
            parts = array_size.split(op)
            length = len(parts)
            for i, part in enumerate(parts):
                if part.isdigit():
                    result += part.strip()
                else:
                    result += process_name(part.strip())
                if i != length - 1:
                    result += ' ' + op + ' '
            return result
    else:
        return process_name(array_size)

def process_enum_name(name):
    if ' ' in name:
        new_name = name.replace(' ', '_')
    elif '_' not in name:
        for uppercase_char in string.ascii_uppercase:
            new_name = name.replace(uppercase_char, '_' + uppercase_char)
    else:
        new_name = name
    return new_name.upper()

ENUM_TYPES = set()
ENUM_PREFIXES = dict()
COMPOUND_TYPES = set()
BITFIELD_TYPES = set()
BITFIELD_STORAGE = dict()
#FLAG_TYPES = set()



class EnumOption:

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

    def __init__(self, element, is_flags=False):
        self.name = element.getAttribute('name')
        ENUM_TYPES.add(self.name)
        self.type = basic_types_n[element.getAttribute('storage')]
        self.description = element.firstChild.nodeValue.strip()
        
        if element.hasAttribute('prefix'):
            self.prefix = element.getAttribute('prefix')
            ENUM_PREFIXES[self.name] = self.prefix + '_'
        else:
            self.prefix = None
            ENUM_PREFIXES[self.name] = ''

        self.options = list()
        for option in element.getElementsByTagName('option'):
            self.options.append(EnumOption(option, self.prefix, is_flags))

    def _get_declaration(self):
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
        return """
        #pragma once
        #include <cstdint>
        """

    def get_hpp(self):
        return self._get_declaration()



def get_enums(document, version):
    enum_hpp = """
    #pragma once

    namespace NiEnums {
    """
    for element in document.getElementsByTagName('enum'):
        if element.hasAttribute('versions'):
            if not version.id in element.getAttribute('versions'):
                continue

        enum = EnumCode(element, is_flags=False)
        enum_hpp += enum.get_hpp()

    for element in document.getElementsByTagName('bitflags'):
        if element.hasAttribute('versions'):
            if not version.id in element.getAttribute('versions'):
                continue

        enum = EnumCode(element, is_flags=True)
        enum_hpp += enum.get_hpp()

    enum_hpp += '}; // namespace\n'
    return enum_hpp

class BitfieldMember:

    def __init__(self, element):
        self.name = process_name(element.getAttribute('name'))
        self.type = element.getAttribute('type')
        if self.type in basic_types_n:
            self.type = basic_types_n[self.type]
        elif self.type in ENUM_TYPES:
            self.type = 'NiEnums::' + self.type

        if element.firstChild is not None:
            self.description = element.firstChild.nodeValue.strip()
        else:
            self.description = ''

        self.width = element.getAttribute('width')
        self.mask = element.getAttribute('mask')
        self.position = element.getAttribute('pos')


        self.declaration = '\t' + self.type + ' ' + self.name + ' : ' + self.width + '; // ' + self.mask + '\n'


    def get_bit_and(self, name, result_name):
        return '\t\t' + result_name +' += ((' + self.name +' & (' + name + ' >> ' + self.position + ')) << ' + self.position + ') & ' + self.mask + ';\n'

    def get_assign(self, name):
        if 'NiEnums::' in self.type:
            return '\t' + self.name + ' = static_cast<'+self.type+'>(' + name + ' & ' + self.mask + ');\n'
        else:
            return '\t' + self.name + ' = ' + name + ' & ' + self.mask + ';\n'

class BitfieldCode:

    def __init__(self, element):
        self.name = element.getAttribute('name')
        self.description = element.firstChild.nodeValue.strip()
        self.type = basic_types_n[element.getAttribute('storage')]
        BITFIELD_TYPES.add(self.name)
        BITFIELD_STORAGE[self.name] = self.type

        self.members = list()
        for member in element.getElementsByTagName('member'):
            self.members.append(BitfieldMember(member))


    def _get_declaration(self):
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
        out = """
        #pragma once
        #include "enums.hpp"
        #include "typedefs.hpp"
        """
        return out

    def get_hpp(self):
        out = self._get_declaration()
        return out


def get_bitfields(document, version):
    bitfields_hpp = """
    #pragma once
    #include "enums.hpp"
    """

    for element in document.getElementsByTagName('bitfield'):
        if element.hasAttribute('versions'):
            if not version.id in element.getAttribute('versions'):
                continue

        bitfield = BitfieldCode(element)
        bitfields_hpp += bitfield.get_hpp()

    return bitfields_hpp


string_types = set(['SizedString', 'SizedString16', 'ExportString'])



def is_correct_version(field):
    if field.hasAttribute('ver1'):
        if version.num < version.get_value(field.getAttribute('ver1')):
            return False
    if field.hasAttribute('ver2'):
        if version.num > version.get_value(field.getAttribute('ver2')):
            return False
    if field.hasAttribute('vercond'):
        if not bool(Expression(field.getAttribute('vercond')).eval(version)):
            return False
    return True


structs_includes_header = """
#pragma once
#include "typedefs.hpp"
#include "enums.hpp"
#include "utils.hpp"
#include "bitfields.hpp"
#include <cstdint>
"""



factory_prefix = """
#pragma once
#include "NiMain.hpp"
#include "NiAnimation.hpp"
#include "NiMesh.hpp"
#include "NiParticle.hpp"
#include "NiLegacy.hpp"
#include "NiPhysX.hpp"
#include "NiPSParticle.hpp"
#include "NiCustom.hpp"
#include "BSMain.hpp"
#include "BSAnimation.hpp"
#include "BSParticle.hpp"
#include "BSLegacy.hpp"
#include "BSHavok.hpp"
#include "typedefs.hpp"
#include "utils.hpp"
#include "structs.hpp"
#include "bitfields.hpp"
#include "enums.hpp"

#include <unordered_map>

struct NiFunctionMaps {
private:
    static std::unordered_map<std::string, NiObject* (*)(FILE*)> createCreatorMap() {
        std::unordered_map<std::string, NiObject* (*)(FILE*)> m;
"""

factory_factory = ''

factory_suffix = """
        return m;
    }
public:
    static const std::unordered_map<std::string, NiObject* (*)(FILE*)> creatorMap;
};

const std::unordered_map<std::string, NiObject* (*)(FILE*)> NiFunctionMaps::creatorMap = createCreatorMap();
"""



basic_types_n = {
    'char' : 'int8_t',
    'byte' : 'uint8_t',
    'short' : 'int16_t',
    'ushort' : 'uint16_t',
    'int' : 'int32_t',
    'uint' : 'uint32_t',
    'ulittle32' : 'uint32_t',
    'int64' : 'int64_t',
    'uint64' : 'uint64_t',
    'float' : 'float',
    'hfloat' : 'float',
    'bool' : 'nif_bool_t',
    'BlockTypeIndex' : 'int16_t',
    'FileVersion' : 'uint32_t',
    'StringOffset' : 'uint32_t',
    'NiFixedString' : 'uint32_t',
    'HeaderString' : 'std::string',
    'LineString' : 'std::string'
}

pointer_types_n = set(['Ptr', 'Ref'])


class PointerType:

    def __init__(self, element):
        self.target_type = element.getAttribute('template')
        self.is_array = False
        name = process_name(element.getAttribute('name'))
        if element.hasAttribute('arr1'):
            size = process_array_size(element.getAttribute('arr1'))
            self.declaration_type = '\tNifPointerList<' + self.target_type + '> ' + name + ';\n'
            self.read_code = name + '.load(reader, ' + size + ');\n'
        else:
            self.declaration_type = '\tNifPointer<' + self.target_type + '> ' + name + ';\n'
            self.read_code = name + '.load(reader);\n'

        
        self.resolve_code = 'this->' + name + '.resolve(data);\n'
        self.delete_code = ''

    def get_type_name(self):
        return self.target_type

class BasicType:

    def __init__(self, element):
        f_type = element.getAttribute('type')
        if f_type in basic_types_n:
            true_type = basic_types_n[f_type]
        elif f_type in ENUM_TYPES:
            true_type = 'NiEnums::' + f_type
        elif f_type in BITFIELD_TYPES:
            true_type = f_type
        else:
            raise ValueError('Invalid type: ' + f_type)

        self.type_name = f_type

        name = process_name(element.getAttribute('name'))
        if element.hasAttribute('arr1'):
            self.is_array = True
            size = process_array_size(element.getAttribute('arr1'))
            self.size = size
            if size.isdecimal():
                self.fixed_array = True
                self.declaration_type = '\t' + true_type + ' ' + name + '[' + size + '];\n'
                self.read_code = '\treader.read(' + name + ', sizeof(' + true_type + '), ' + size + ');\n'
                self.delete_code = ''
            else:
                self.fixed_array = False
                self.declaration_type = '\t' + true_type + '* ' + name + ';\n'
                self.read_code = '\t' + name + ' = new ' + true_type + '[' + size + '];\n'
                self.read_code += '\treader.read(' + name + ', sizeof(' + true_type + '), ' + size + ');\n'
                self.delete_code = '\tdelete[]' + name + ';\n'
            self.resolve_code = ''
        else:
            self.is_array = False
            self.declaration_type = '\t' + true_type + ' ' + name + ';\n'
            self.read_code = '\treader.read(&' + name + ', sizeof(' + true_type + '), 1);\n'
            self.delete_code = ''
            self.resolve_code = ''


    def get_type_name(self):
        return self.type_name


class StringType:

    def __init__(self, element):
        f_type = element.getAttribute('type')
        name = process_name(element.getAttribute('name'))

        if f_type == 'string':
            self.declaration_type = '\tchar* ' + name + ';\n'
            self.read_code = '\t' + name + ' = reader.readIndexedString();\n'
            self.delete_code = '\tdelete[] ' + name + ';\n'
            self.resolve_code = ''

        else:
            if element.hasAttribute('arr1'):
                self.is_array = True
                self.size = process_array_size(element.getAttribute('arr1'))
                
                self.read_code = ''
                self.delete_code = '\tfor(unsigned int i = 0; i < ' + self.size + '; i++) {\n'
                self.delete_code += '\t\tdelete ' + name + '[i];\n'
                self.delete_code += '\t}\n'

                
                if self.size.isdecimal():
                    self.fixed_array = True
                    self.declaration_type = '\tchar* ' + name + '[' + self.size + '];\n'
                    
                else:
                    self.fixed_array = False
                    self.declaration_type = '\tchar** ' + name + ';\n'
                    self.read_code = '\t' + name + ' = new char*[' + self.size + '];\n'
                    self.delete_code += '\tdelete[] ' + name + ';\n'

                self.read_code += '\tfor(unsigned int i = 0; i < ' + self.size + '; i++) {\n'
                self.read_code += '\t\t' + name + '[i] = reader.loadSizedString();\n'
                self.read_code += '\t}\n'
                
                
                self.resolve_code = ''
            else:
                self.declaration_type = '\tchar* ' + name + ';\n'
                self.read_code = '\t' + name + ' = reader.loadSizedString();\n'
                self.delete_code = '\tdelete[] ' + name + ';\n'
                self.resolve_code = ''

    def get_type_name(self):
        return 'char*'


class CompoundType:

    def __init__(self, element):
        name = process_name(element.getAttribute('name'))
        true_type = element.getAttribute('type')
        if true_type == '#T#':
            true_type = 'T'

        if element.hasAttribute('template'):
            true_type += '<T>'

        self.type_name = true_type

        if element.hasAttribute('arr1'):
            self.is_array = True
            self.fixed_array = False
            size = process_array_size(element.getAttribute('arr1'))
            self.size = size
            self.declaration_type = '\tstd::vector<' + true_type + '> ' + name + ';\n'
            self.read_code = '\t' + name + '.reserve(' + size + ');\n'
            self.read_code += '\tfor(unsigned int i = 0; i < ' + size + '; i++) {\n'
            self.read_code += '\t\t' + name + '.emplace_back(reader);\n'
            self.read_code += '\t}\n' 
            self.delete_code = ''
            self.resolve_code = '\tfor(unsigned int i = 0; i < ' + size + '; i++) {\n'
            self.resolve_code += '\t\t' + name + '[i].resolvePointers(data);\n'
            self.resolve_code += '\t}\n' 

        else:
            self.is_array = False
            self.declaration_type = '\t' + true_type + '* ' + name + ';\n'
            self.read_code = '\t' + name + ' = new ' + true_type + '(reader);\n'
            self.resolve_code = '\t' + name + '->resolvePointers(data);\n'
            self.delete_code = '\tdelete ' + name + ';\n'

    def get_type_name(self):
        return self.type_name

class FieldCode:

    def __init__(self, element):
        self.name = process_name(element.getAttribute('name'))

        if element.firstChild is not None:
            self.description = element.firstChild.nodeValue.strip()
        else:
            self.description = None

        self.hasCondition = element.hasAttribute('cond')
        if self.hasCondition:
            self.condition = str(Expression(element.getAttribute('cond'), name_filter=process_name))

        f_type = element.getAttribute('type')
        if f_type in pointer_types_n:
            self.type = PointerType(element)
        elif f_type in basic_types_n or f_type in ENUM_TYPES or f_type in BITFIELD_TYPES:
            self.type = BasicType(element)
        elif f_type in ['string', 'SizedString']:
            self.type = StringType(element)
        else:
            self.type = CompoundType(element)

    def get_declaration(self):
        return self.type.declaration_type

    def get_constructor_code(self):
        out = ''
        back = ''
        if self.hasCondition:
            if self.type.is_array and not self.type.fixed_array:
                out += 'if(' + self.condition + ' && ' + self.type.size + ' != 0) {\n'
            else:
                out += 'if(' + self.condition + ') {\n'
            back += '}\n'
        out += self.type.read_code
        out += back
        return out

    def get_destructor_code(self):
        out = ''
        back = ''
        if self.hasCondition:
            if self.type.is_array and not self.type.fixed_array:
                out += 'if(' + self.condition + ' && ' + self.type.size + ' != 0) {\n'
            else:
                out += 'if(' + self.condition + ') {\n'
            back += '}\n'
        out += self.type.delete_code
        out += back
        return out

    def get_resolve_pointers_code(self):
        out = ''
        out += self.type.resolve_code
        return out

    def get_type(self):
        return self.type.get_type_name()


class CompoundCode:

    def __init__(self, element):
        self.name = element.getAttribute('name')
        COMPOUND_TYPES.add(self.name)
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
            if is_correct_version(el):
                self.fields.append(FieldCode(el))


    @classmethod
    def _get_includes(cls):
        return """
        #pragma once
        #include "typedefs.hpp"
        #include "enums.hpp"
        #include "utils.hpp"
        #include "bitfields.hpp"
        #include <cstdint>
        #include <vector>
        """

    def _get_declaration(self):
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
        if self.generic:
            out = 'template <typename T>\n ' + self.name + '<T>::' + self.name + '(NifReader& reader) {\n'
        else:
            out = self.name + '::' + self.name + '(NifReader& reader) {\n'
        for field in self.fields:
            out += field.get_constructor_code()
        out += '}\n'
        return out

    def _get_destructor(self):
        if self.generic:
            out = 'template <typename T>\n ' + self.name + '<T>::~' + self.name + '() {\n'
        else:
            out = self.name + '::~' + self.name + '() {\n'
        for field in self.fields:
            out += field.get_destructor_code()
        out += '}\n'
        return out

    def _get_resolve_pointers(self):
        if self.generic:
            out = 'template <typename T>\nvoid ' + self.name + '<T>::resolvePointers(NifData& data) {\n'
        else:
            out = 'void ' + self.name + '::resolvePointers(NifData& data) {\n'
        for field in self.fields:
            out += field.get_resolve_pointers_code()
        out += '}\n'
        return out

    def get_hpp(self):
        return self._get_declaration()
        
    def get_cpp(self):
        out = self._get_constructor()
        out += self._get_destructor()
        out += self._get_resolve_pointers()
        return out


def get_compounds(document, version):
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

        compound = CompoundCode(element)
        structs_hpp += compound.get_hpp()
        structs_cpp += compound.get_cpp()

    return structs_hpp, structs_cpp


class ObjectCode:

    def __init__(self, element):
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
            if is_correct_version(el):
                self.fields.append(FieldCode(el))

    def _get_includes(self):
        out = """
        #pragma once
        #include "typedefs.hpp"
        #include "enums.hpp"
        #include "bitfields.hpp"
        #include "structs.hpp"
        #include "../nifreader.hpp"
        #include "../nifpointer.hpp"
        class NifData;
        """
        if self.parent is not None:
            out += 'struct ' + self.parent + ';\n'
            out += '#include "' + self.parent + '.hpp"\n'


        has_vector = False
        for field in self.fields:
            #type_name = field.get_type()
            #if type_name not in COMPOUND_TYPES and \
             #   type_name not in ENUM_TYPES and \
             #   type_name not in BITFIELD_TYPES:

             #   out += 'struct ' + type_name + ';\n'
             #   out += '#include "' + type_name + '.hpp"\n'
             if type(field.type) == PointerType:
                out += 'struct ' + field.type.target_type + ';\n'
                out += '#include "' + field.type.target_type + '.hpp"\n'
             elif type(field.type) == CompoundType:
                if field.type.is_array:
                    out += '#include <vector>\n'

        out += '#include <cstdint>\n'
        out += '\n'

        return out

    def _get_declaration(self):
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
        if self.parent is not None:
            out = self.name + '::' + self.name + '(NifReader& reader) : ' + self.parent + '(reader) {\n'
        else:
            out = self.name + '::' + self.name + '(NifReader& reader) {\n'

        for field in self.fields:
            out += field.get_constructor_code()
        out += '}\n'
        return out

    def _get_destructor(self):
        out = self.name + '::~' + self.name + '() {\n'
        for field in self.fields:
            out += field.get_destructor_code()
        out += '}\n'
        return out

    def _get_resolve_pointers(self):
        out = 'void ' + self.name + '::resolvePointers(NifData& data) {\n'
        if self.parent is not None:
            out += '\t' + self.parent + '::resolvePointers(data);\n'
        for field in self.fields:
            out += field.get_resolve_pointers_code()
        out += '}\n'
        return out

    def _get_factory(self):
        out = 'NiObject* ' + self.name + '::create(NifReader& reader) {\n'
        out += '\treturn new ' + self.name + '(reader);\n'
        out += '}\n'
        return out

    def get_hpp(self):
        out = self._get_includes()
        out += self._get_declaration()
        return out
        
    def get_cpp(self):
        out = '#include "' + self.name + '.hpp"\n'
        out += self._get_constructor()
        out += self._get_factory()
        out += self._get_destructor()
        out += self._get_resolve_pointers()
        return out
        


class ObjectModule:

    def __init__(self, module_elements):
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
            obj = ObjectCode(module_element)
            self.module_hpps[name] = obj.get_hpp()
            self.module_cpps[name] = obj.get_cpp()


    def get_module_codes(self):
        for key in self.module_hpps.keys():
            yield key, self.module_hpps[key], self.module_cpps[key]



def get_module(document, version, module_name):
    module = ObjectModule([element for element in document.getElementsByTagName('niobject') if element.getAttribute('module') == module_name])
    return module



includes_header = """
#pragma once
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include "typedefs.hpp"
#include "utils.hpp"
#include "structs.hpp"
#include "bitfields.hpp"
#include "enums.hpp"
"""

def get_module_names(document):
    modules = list()
    for module in document.getElementsByTagName('module'):
        name = module.getAttribute('name')
        #modules[name] = includes_header
        #if module.hasAttribute('depends'):
        #    for dependency in module.getAttribute('depends').split(' '):
        #        modules[name] += '#include "' + dependency + '.hpp"\n'
        modules.append(module.getAttribute('name'))
    return modules


def get_typedefs():
    return """
    #pragma once
    #include <cstdint>
    typedef uint8_t nif_bool_t;
    """



if __name__ == '__main__':
    path = 'nif.xml'
    if os.path.exists(path):
        document = xml.dom.minidom.parse(path)
    else:
        raise ImportError("nif.xml not found")

    document = replace_tokens(document)

    versions = create_versions(document, 'Fallout NV')

    for version in versions:
        if not os.path.exists(version.id):
            os.mkdir(version.id)


        typedefs = get_typedefs()
        with open(str(version.id) + '/typedefs.hpp', 'w', encoding='utf-8') as f:
            f.write(typedefs)

        enums = get_enums(document, version)
        with open(str(version.id) + '/enums.hpp', 'w', encoding='utf-8') as f:
            f.write(enums)

        bitfields = get_bitfields(document, version)
        with open(str(version.id) + '/bitfields.hpp', 'w', encoding='utf-8') as f:
            f.write(bitfields)

        compounds_hpp, compounds_cpp = get_compounds(document, version)
        with open(str(version.id) + '/structs.hpp', 'w', encoding='utf-8') as f:
            f.write(compounds_hpp)

        with open(str(version.id) + '/structs.cpp', 'w', encoding='utf-8') as f:
            f.write(compounds_cpp)

        module_names = get_module_names(document)


        for module_name in module_names:
            module = get_module(document, version, module_name)
            if not os.path.exists(str(version.id) + '/' + module_name):
                os.mkdir(str(version.id) + '/' + module_name)

            for name, hpp, cpp in module.get_module_codes():
                with open(str(version.id) + '/' + module_name + '/' + name + '.hpp', 'w', encoding='utf-8') as f:
                    f.write(hpp)
                with open(str(version.id) + '/' + module_name + '/' + name + '.cpp', 'w', encoding='utf-8') as f:
                    f.write(cpp)

        with open(str(version.id) + '/factory.hpp', 'w', encoding='utf-8') as f:
            f.write(factory_prefix+factory_factory+factory_suffix)