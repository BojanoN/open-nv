import os
import xml.dom
import xml.dom.minidom
import re
import sys
import string

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
BITFIELD_TYPES = set()

def get_enums(document, version, flags=False):
    generated_enums = '#pragma once\n\nnamespace NiEnums {\n'
    if flags:
        tag_name = 'bitflags'
    else:
        tag_name = 'enum'
    for enum in document.getElementsByTagName(tag_name):
        name = enum.getAttribute('name')
        #if name == 'Fallout3HavokMaterial':
        #    breakpoint()
        if enum.hasAttribute('versions'):
            if not version.id in enum.getAttribute('versions'):
                continue
        

        ENUM_TYPES.add(name)
        enum_type = basic_types[enum.getAttribute('storage')]
        description = enum.firstChild.nodeValue.strip()
        prefix = enum.getAttribute('prefix') if enum.hasAttribute('prefix') else None
        if prefix is not None:
            ENUM_PREFIXES[name] = prefix + '_'
        else:
            ENUM_PREFIXES[name] = ''
        new_enum = ''
        new_enum += '/*\n' + description + '\n*/\n'
        new_enum += 'enum ' + name + ' : ' + enum_type + ' {\n'

        print_func = ''
        print_func += 'void enumPrint' + name + '(' + name + ' value) {\n'
        if not flags:
            print_func += '\tswitch(value){\n'

        options = enum.getElementsByTagName('option')
        for i, option in enumerate(options):
            #option_name = process_name(option.getAttribute('name'))
            option_name = process_enum_name(option.getAttribute('name'))
            if prefix is not None:
                option_name = prefix + '_' + option_name

            if flags:
                value = '0x{:08X}'.format(1 << int(option.getAttribute('bit')))
            else:
                value = option.getAttribute('value')

            if option.firstChild is not None:
                option_desc = option.firstChild.nodeValue.strip()
            else:
                option_desc = None

            new_enum += '\t' + option_name + ' = ' + value
            if i != len(options) - 1:
                 new_enum += ','

            if option_desc is not None:
                new_enum += ' // ' + option_desc + '\n'
            else:
                new_enum += '\n'
            
            if flags:
                print_func += '\tstd::printf("' + option_name + ': %s\\n", (value & ' + value + ')? "set" : "not set");\n'
            else:
                print_func += '\tcase ' + option_name + ': std::printf("' + option_name + '\\n"); break;\n'

        if not flags:
            print_func += '\t}\n'
        new_enum += '};\n\n'
        print_func += '}\n'
        new_enum += print_func
        #print(new_enum)
        generated_enums += new_enum
    generated_enums += '};\n'
    return generated_enums


def get_bitfields(document, version):
    generated_bitfields = ''
    generated_bitfields += '#pragma once\n#include "enums.hpp"\n\n'
    for bitfield in document.getElementsByTagName('bitfield'):
        if bitfield.hasAttribute('versions'):
            if not version.id in bitfield.getAttribute('versions'):
                continue


        name = bitfield.getAttribute('name')
        description = bitfield.firstChild.nodeValue.strip()
        storage = basic_types[bitfield.getAttribute('storage')]

        BITFIELD_TYPES.add(name)
        new_bitfield = ''
        new_bitfield += '/*\n' + description + '\nStorage type: ' + storage + '\n*/\n'
        new_bitfield += 'struct ' + name + ' {\n'
        print_func = 'void ' + name + '::print() {\n'
        assign_op = name + '& ' + name + '::operator=(const ' + storage + '& value) {\n'
        bitwise_and_op = '\t' + storage + ' operator&(const ' + storage + '& b) const {\n'
        bitwise_and_op += '\t\t' + storage + ' result = 0;\n'
        for member in bitfield.getElementsByTagName('member'):
            member_name = process_name(member.getAttribute('name'))
            member_type = member.getAttribute('type')
            if member_type in basic_types:
                member_type = basic_types[member_type]

            member_width = member.getAttribute('width')
            member_mask = member.getAttribute('mask')
            member_pos = member.getAttribute('pos')

            if member.firstChild is not None:
                member_desc = member.firstChild.nodeValue.strip()
            else:
                member_desc = None

            if member_desc is not None:
                desc_parts = member_desc.split('\n')
                first = True
                for part in desc_parts:
                    new_bitfield += '\t// ' + part + '\n'

            if member_type in ENUM_TYPES:
                new_bitfield += '\tNiEnums::' + member_type + ' ' + member_name + ' : ' + member_width + '; // ' + member_mask + '\n'
            else:
                new_bitfield += '\t' + member_type + ' ' + member_name + ' : ' + member_width + '; // ' + member_mask + '\n'
            if member_type in basic_types.values():
                print_func += '\tstd::printf("' + member_name + '(width: ' + member_width +'): ' + string_format_ids[member_type] + '\\n", ' + member_name + ');\n'
                assign_op += '\t' + member_name + ' = value & ' + member_mask + ';\n'
            else:
                print_func += '\tstd::printf("' + member_name + '(width: ' + member_width +'): ");\n'
                print_func += '\tNiEnums::enumPrint' + member_type + '(' + member_name + ');\n'
                assign_op += '\t' + member_name + ' = static_cast<NiEnums::'+member_type+'>(value & ' + member_mask + ');\n'
            bitwise_and_op += '\t\tresult += ((' + member_name +' & (b >> '+member_pos+')) << ' + member_pos + ') & '+member_mask+';\n'

        new_bitfield += '\n\n\tvoid load(FILE* file);\n'
        new_bitfield += '\tvoid print();\n'
        new_bitfield += '\t' + name + '& operator=(const ' + storage + '& value);\n'
        new_bitfield += '\t' + name + '& operator=(int value) { return operator=(static_cast<'+storage+'>(value)); }\n'
        
        bitwise_and_op += '\treturn result;\n\t}\n'
        new_bitfield += bitwise_and_op
        new_bitfield += '};\n'
        loader_func = 'void ' + name + '::load(FILE* file) {\n'
        loader_func += '\tstd::fread(this, sizeof(' + storage + '), 1, file);\n}\n'
        print_func += '}\n'
        assign_op += 'return *this;\n}\n'
        new_bitfield += loader_func
        new_bitfield += print_func
        new_bitfield += assign_op
        generated_bitfields += new_bitfield
    return generated_bitfields


load_string_func = """
void loadString(FILE* file, std::string& target) {
    unsigned int index = 0;
    do {
        int c = std::fgetc(file);
        if(c == '\\n') {
            break;
        } else {
            globalStringBuffer[index++] = c;
        }
    } while(true);
    target.reserve(index + 1);
    std::memcpy(&target[0], globalStringBuffer, index + 1);
}
"""

load_sized_string_func = """
void loadSizedString(FILE* file, std::string& target) {
    uint32_t length;
    std::fread(&length, sizeof(uint32_t), 1, file);
    target.reserve(length + 1);
    std::fread(&target[0], sizeof(uint8_t), length, file);
    target[length] = '\\0';
}
"""
load_sized_string_16_func = """
void loadSizedString16(FILE* file, std::string& target) {
    uint16_t length;
    std::fread(&length, sizeof(uint16_t), 1, file);
    target.reserve(length + 1);
    std::fread(&target[0], sizeof(uint8_t), length, file);
    target[length] = '\\0';
}
"""

load_export_string_func = """
void loadExportString(FILE* file, std::string& target) {
    uint8_t length;
    std::fread(&length, sizeof(uint8_t), 1, file);
    target.reserve(length + 1);
    std::fread(&target[0], sizeof(uint8_t), length, file);
}
"""

string_types = set(['SizedString', 'SizedString16', 'ExportString'])


def load_fixed_vector(field_name, array_size):
    loader_func = '\tfor(unsigned int i = 0, limit = ' + array_size + '; i < limit; i++) {\n'
    loader_func += '\t\t' + field_name + '[i].load(file);\n'
    loader_func += '\t}\n'
    return loader_func

def print_basic_vector(field_name, field_type, array_size, is_fixed=True):
    print_func = ''
    if is_fixed:
        print_func += '\tstd::printf("' + field_name + '; array of size: ' + array_size + '\\n");\n'
    else:
        print_func += '\tstd::printf("' + field_name + '; array of size: %u\\n", ' + array_size + ');\n'
    print_func += '\tfor(unsigned int i = 0, limit = ' + array_size + '; i < limit; i++) {\n'
    if field_type in string_types:
        print_func += '\t\tstd::printf("' + field_name + '[%u]: %s\\n", i, ' + field_name + '[i].c_str());\n'
    elif field_type in basic_types.values():
        print_func += '\t\tstd::printf("' + field_name + '[%u]: ' + string_format_ids[field_type] + '\\n", i, ' + field_name + '[i]);\n'
    elif field_type in ENUM_TYPES:
        print_func += '\t\tstd::printf("' + field_name + '[%u]:\\n", i);\n'
        print_func += '\t\tNiEnums::enumPrint' + field_type + '('+field_name+'[i]);\n'
    else:
        print_func += '\t\tstd::printf("' + field_name + '[%u]:\\n", i);\n'
        print_func += '\t\t' + field_name + '[i].print();\n'

    print_func += '\t}\n'
    return print_func

def load_string_vector(field_name, field_type, array_size):
    loader_func = '\t' + field_name + '.reserve(' + array_size + ');\n'
    loader_func += '\tfor(unsigned int i = 0, limit = ' + array_size + '; i < limit; i++) {\n'
    loader_func += '\t\t' + field_name + '.emplace_back();\n'
    loader_func += '\t\tload' + field_type + '(file, ' + field_name + '.back());\n'
    loader_func += '\t}\n'
    return loader_func

def load_basic_vector(field_name, field_type, array_size):
    cur_type = 'NiEnums::'+field_type if field_type in ENUM_TYPES else field_type
    loader_func = '\t' + field_name + '.reserve(' + array_size + ' * sizeof(' + cur_type + '));\n'
    loader_func += '\tstd::fread(&' + field_name + '[0], sizeof(' + cur_type + '), ' + array_size + ', file);\n'
    return loader_func

def load_vector(field_name, array_size):
    loader_func = '\t' + field_name + '.reserve(' + array_size + ');\n'
    loader_func += '\tfor(unsigned int i = 0, limit = ' + array_size + '; i < limit; i++) {\n'
    loader_func += '\t\t' + field_name + '.emplace_back();\n'
    loader_func += '\t\t' + field_name + '.back().load(file);\n'
    loader_func += '\t}\n'
    return loader_func


def load_array(field_name, field_type, array_size, field):
    new_compound = ''
    loader_func = ''
    print_func = ''
    if array_size.isdecimal():
        actual_size = int(array_size)
        new_compound += '\t' + field_type + ' ' + field_name + '[' + str(actual_size) + '];\n'
        if field_type in basic_types.values():
            loader_func += '\tstd::fread(&' + field_name + ', sizeof(' + field_type + '), ' + str(actual_size) + ', file);\n'
        else:
            loader_func += load_fixed_vector(field_name, array_size)
        print_func += print_basic_vector(field_name, field_type, array_size, is_fixed=True)

    else:
        new_compound += '\t// Size: ' + array_size + '\n'
        if field_type in string_types:
            new_compound += '\tstd::vector<std::string> ' + field_name + ';\n'
        elif field_type in ENUM_TYPES:
            new_compound += '\tstd::vector<NiEnums::' + field_type + '> ' + field_name + ';\n'
        else:
            new_compound += '\tstd::vector<' + field_type + '> ' + field_name + ';\n'

        #array_size = ''.join(array_size.split(sep=' '))
        if field_type in basic_types.values() or field_type in ENUM_TYPES:
            loader_func += load_basic_vector(field_name, field_type, array_size)
        elif field_type in string_types:
            loader_func += load_string_vector(field_name, field_type, array_size)
        else:
            loader_func += load_vector(field_name, array_size)
        print_func += print_basic_vector(field_name, field_type, array_size, is_fixed=False)
    return new_compound, loader_func, print_func

def get_prefix(enum_type):
    if enum_type not in ENUM_TYPES:
        raise ValueError(str(enum_type) + ' is not an enum type!')
    return ENUM_PREFIXES[enum_type]

def load_single(field_name, field_type, field):
    new_compound = ''
    loader_func = ''
    print_func = ''
    if field_type in string_types:
        new_compound += '\tstd::string ' + field_name
    elif field_type in ENUM_TYPES:
        new_compound += '\tNiEnums::' + field_type + ' ' + field_name
    else:
        new_compound += '\t' + field_type + ' ' + field_name
    if field.hasAttribute('default'):
        if field_type in ENUM_TYPES:
            default_value = field.getAttribute('default')
            if default_value.isdigit() or default_value.startswith('0x'):
                new_compound += ' = static_cast<NiEnums::' + field_type + '>(' + field.getAttribute('default') + ');\n'
            else:
                new_compound += ' = NiEnums::' + get_prefix(field_type) + default_value + ';\n'
        elif field_type == 'string' and not field.getAttribute('default').isdigit() or field_type in BITFIELD_TYPES:
                new_compound += ';\n'
        elif field_type in basic_types.values() or field_type in BITFIELD_TYPES:
            new_compound += ' = ' + field.getAttribute('default') + ';\n'
        else:
            new_compound += ' = {' + field.getAttribute('default') + '};\n'
    else:
        new_compound += ';\n'

    if field_type == 'std::string':
        loader_func += '\tloadString(file, ' + field_name + ');\n'
    elif field_type == 'SizedString':
        loader_func += '\tloadSizedString(file, ' + field_name + ');\n'
    elif field_type == 'SizedString16':
        loader_func += '\tloadSizedString16(file, ' + field_name + ');\n'
    elif field_type == 'ExportString':
        loader_func += '\tloadExportString(file, ' + field_name + ');\n'
    elif field_type in basic_types.values() or field_type == 'T':
        loader_func += '\tstd::fread(&' + field_name + ', sizeof(' + field_type + '), 1, file);\n'
    elif field_type in ENUM_TYPES:
        loader_func += '\tstd::fread(&' + field_name + ', sizeof(NiEnums::' + field_type + '), 1, file);\n'
    else:
        loader_func += '\t' + field_name + '.load(file);\n'

    if field_type in basic_types.values():
        print_func += '\tstd::printf("' + field_name + ': ' + string_format_ids[field_type] + '\\n", ' + field_name + ');\n'
    elif field_type == 'T':
        print_func += '\tstd::printf("' + field_name + ': %u\\n", ' + field_name + ');\n'
    elif field_type in string_types or field_type == 'std::string':
        print_func += '\tstd::printf("' + field_name + ': %s\\n", ' + field_name + '.c_str());\n'
    elif field_type in ENUM_TYPES:
        print_func += '\tstd::printf("' + field_name + ':\\n");\n'
        print_func += '\tNiEnums::enumPrint' + field_type + '(' + field_name + ');\n'
    else:
        print_func += '\tstd::printf("' + field_name + ':\\n");\n'
        print_func += '\t' + field_name + '.print();\n'

    return new_compound, loader_func, print_func


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

def get_compounds(document, version):
    generated_compounds = ''
    generated_compounds += structs_includes_header
    for compound in document.getElementsByTagName('compound'):
        if compound.hasAttribute('versions'):
            if not version.id in compound.getAttribute('versions'):
                continue
        if compound.hasAttribute('until'):
            if not version.is_before(compound.getAttribute('until')):
                continue
        if compound.hasAttribute('since'):
            if not version.is_after(compound.getAttribute('since')):
                continue

        name = compound.getAttribute('name')
        if compound.hasAttribute('generic') and compound.getAttribute('generic') == 'true':
            template = True
        else:
            template = False
        description = compound.firstChild.nodeValue.strip()
        new_compound = ''
        new_compound += '/*\n' + description
        if compound.hasAttribute('size'):
            new_compound += '\nSize: ' + compound.getAttribute('size') + '\n*/\n'
        else:
            new_compound += '\n*/\n'
        if template:
            new_compound += 'template <typename T>\n'
            loader_func = 'template <typename T>\nvoid ' + name + '<T>::load(FILE* file) {\n'
            print_func = 'template <typename T>\nvoid ' + name + '<T>::print() {\n'
        else:
            loader_func = 'void ' + name + '::load(FILE* file) {\n'
            print_func = 'void ' + name + '::print() {\n'
        new_compound += 'struct ' + name + ' {\n'
        
        

        fields = compound.getElementsByTagName('field')
        for field in fields:
            if not is_correct_version(field):
                continue

            field_name = process_name(field.getAttribute('name'))
            field_type = field.getAttribute('type')
            if field_type in basic_types:
                field_type = basic_types[field_type]
            if field.hasAttribute('template') and field_type not in ('nif_ptr_t', 'nif_ref_t'):
                field_type += '<T>'
            if field_type == '#T#':
                field_type = 'T'

            if field.firstChild is not None:
                field_desc = field.firstChild.nodeValue.strip()
            else:
                field_desc = None
            if field.hasAttribute('cond'):
                new_compound += '\t// Condition: ' + field.getAttribute('cond') + '\n'
            if field_type == 'nif_ptr_t':
                new_compound += '\t// Ptr to: ' + field.getAttribute('template') + '\n'
            elif field_type == 'nif_ref_t':
                new_compound += '\t// Ref to: ' + field.getAttribute('template') + '\n'

            if field.hasAttribute('range'):
                new_compound += '\t// Range: ' + field.getAttribute('range') + '\n'

            if field_desc is not None:
                desc_parts = field_desc.split('\n')
                first = True
                for part in desc_parts:
                    new_compound += '\t// ' + part + '\n'

            if field.hasAttribute('cond'):
                loader_func += '\tif (' + str(Expression(field.getAttribute('cond'), name_filter=process_name)) + ') {\n'
                print_func += '\tif (' + str(Expression(field.getAttribute('cond'), name_filter=process_name)) + ') {\n'

            if field.hasAttribute('arr1'):
                array_size = process_array_size(field.getAttribute('arr1'))
                compound_ext, loader_ext, print_ext = load_array(field_name, field_type, array_size, field)

            else:
                compound_ext, loader_ext, print_ext = load_single(field_name, field_type, field)
            new_compound += compound_ext
            loader_func += loader_ext
            print_func += print_ext
            if field.hasAttribute('cond'):
                loader_func += '\t}\n'
                print_func += '\t}\n'

        new_compound += '\n\n\tvoid load(FILE* file);\n'
        new_compound += '\tvoid print();\n'
        new_compound += '};\n\n'
        loader_func += '}\n'
        print_func += '}\n\n'
        new_compound += loader_func
        new_compound += print_func
        generated_compounds += new_compound
    return generated_compounds


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


def get_objects(document, version, modules):
    for niobject in document.getElementsByTagName('niobject'):
        if niobject.hasAttribute('versions'):
            if not version.id in niobject.getAttribute('versions'):
                continue
        if niobject.hasAttribute('until'):
            if not version.is_before(niobject.getAttribute('until')):
                continue
        if niobject.hasAttribute('since'):
            if not version.is_after(niobject.getAttribute('since')):
                continue

        module_name = niobject.getAttribute('module')
        new_object = ''
        if niobject.firstChild is not None:
                description = niobject.firstChild.nodeValue.strip()
        else:
            description = None
        new_object += '/*\n' + str(description) +  '\n*/\n'
        name = niobject.getAttribute('name')
        new_object += 'struct ' + name

        loader_func = 'void ' + name + '::load(FILE* file) {\n'
        print_func = 'void ' + name + '::print() {\n'
        if niobject.hasAttribute('inherit'):
            parent = niobject.getAttribute('inherit')
            new_object += ' : public ' + parent + ' {\n'
            loader_func += '\t' + parent + '::load(file);\n'
            print_func += '\t' + parent + '::print();\n\tstd::printf("' + name + ':--------------\\n");\n'
        else:
            new_object += ' {\n'

        fields = niobject.getElementsByTagName('field')
        for field in fields:
            if not is_correct_version(field):
                continue

            field_name = process_name(field.getAttribute('name'))
            field_type = field.getAttribute('type')
            if field_type in basic_types:
                field_type = basic_types[field_type]
            elif field.hasAttribute('template'):
                template_type = field.getAttribute('template')
                if template_type in basic_types:
                    field_type += '<' + basic_types[template_type] + '>'
                else:
                    field_type += '<' + template_type + '>'

            if field.firstChild is not None:
                field_desc = field.firstChild.nodeValue.strip()
            else:
                field_desc = None
            if field.hasAttribute('cond'):
                new_object += '\t// Condition: ' + field.getAttribute('cond') + '\n'
            if field.hasAttribute('range'):
                new_object += '\t// Range: ' + field.getAttribute('range') + '\n'

            if field_desc is not None:
                desc_parts = field_desc.split('\n')
                first = True
                for part in desc_parts:
                    new_object += '\t// ' + part + '\n'

            if field.hasAttribute('arr1'):
                array_size = process_array_size(field.getAttribute('arr1'))
                object_ext, loader_ext, print_ext = load_array(field_name, field_type, array_size, field)
            else:
                object_ext, loader_ext, print_ext = load_single(field_name, field_type, field)

            new_object += object_ext
            loader_func += loader_ext
            print_func += print_ext

        new_object += '\n\n\tvoid load(FILE* file);\n'
        new_object += '\tstatic NiObject* create(FILE* file){\n'
        new_object += '\t\t' + name + '* objPtr = new ' + name + ';\n'
        new_object += '\t\tobjPtr->load(file);\n\t\treturn objPtr;\n\t}\n'
        global factory_factory
        factory_factory += '\t\tm["' + name + '"] = &' + name + '::create;\n'
        new_object += '\tvirtual void print();\n'
        new_object += '};\n\n'
        loader_func += '}\n'
        print_func += '\tstd::printf("---------------\\n");\n'
        print_func += '}\n\n'
        new_object += loader_func
        new_object += print_func
        modules[module_name] += new_object
    return modules

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

def get_modules(document):
    modules = dict()
    for module in document.getElementsByTagName('module'):
        name = module.getAttribute('name')
        modules[name] = includes_header
        if module.hasAttribute('depends'):
            for dependency in module.getAttribute('depends').split(' '):
                modules[name] += '#include "' + dependency + '.hpp"\n'
    return modules


def get_typedefs():
    return """
    #pragma once
    typedef uint32_t nif_ref_t;
    typedef uint32_t nif_ptr_t;
    typedef uint8_t nif_bool_t;
    """

def get_utils():
    utils = ''
    utils += '#pragma once\n\nchar* globalStringBuffer;\n\n'
    utils += load_string_func
    utils += load_sized_string_func
    utils += load_sized_string_16_func
    utils += load_export_string_func
    return utils

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

        utils = get_utils()
        with open(str(version.id) + '/utils.hpp', 'w', encoding='utf-8') as f:
            f.write(utils)

        typedefs = get_typedefs()
        with open(str(version.id) + '/typedefs.hpp', 'w', encoding='utf-8') as f:
            f.write(typedefs)

        enums = get_enums(document, version)
        enums += get_enums(document, version, flags=True)
        with open(str(version.id) + '/enums.hpp', 'w', encoding='utf-8') as f:
            f.write(enums)

        bitfields = get_bitfields(document, version)
        with open(str(version.id) + '/bitfields.hpp', 'w', encoding='utf-8') as f:
            f.write(bitfields)

        compounds = get_compounds(document, version)
        with open(str(version.id) + '/structs.hpp', 'w', encoding='utf-8') as f:
            f.write(compounds)

        modules = get_modules(document)
        objects = get_objects(document, version, modules)
        for module, objects in objects.items():
            with open(str(version.id) + '/' + module + '.hpp', 'w', encoding='utf-8') as f:
                f.write(objects)    

        with open(str(version.id) + '/factory.hpp', 'w', encoding='utf-8') as f:
            f.write(factory_prefix+factory_factory+factory_suffix)