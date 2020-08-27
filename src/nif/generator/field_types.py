from name_utils import process_name, process_array_size
from expression import Expression

class PointerType:
    """
        Represents a pointer field type.

        Includes the pointed-to type, field name, and array size,
        in case there is an array of pointers.

        Attributes:

            target_type (str) -- Pointed-to type.
            is_array (bool) -- If true, the field is an array of pointers.
            declaration_type (str) -- Field declaration line.
            read_code (str) -- Code for the loader function.
            resolve_code (str) -- Code for the pointer resolution function.
            delete_code (str) -- Code for the destructor.
    """

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
        """
            Returns the target type.
        """
        return self.target_type

class BasicType:
    """
        Represents a basic field type.
        A basic field type is a field with a builtin type, or an enum
        or bitfield type.

        Attributes:
            is_array (bool) -- If true, the field is an array.
            fixed_array (bool) -- If true, the field is a fixed-size array.
            declaration_type (str) -- Field declaration line.
            read_code (str) -- Code for the loader function.
            resolve_code (str) -- Code for the pointer resolution function.
            delete_code (str) -- Code for the destructor.
    """

    def __init__(self, context, element):
        f_type = element.getAttribute('type')
        if context.is_basic_type(f_type):
            true_type = context.get_basic_type(f_type)
        elif context.is_enum_type(f_type):
            true_type = 'NiEnums::' + f_type
        elif context.is_bitfield_type(f_type):
            true_type = f_type
        else:
            raise ValueError('Invalid basic, enum or bitfield type: ' + f_type)

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

                if element.hasAttribute('arr2'):
                    size_2 = process_array_size(element.getAttribute('arr2'))
                    self.declaration_type = '\t' + true_type + '** ' + name + ';\n'

                    if size_2.isdecimal():
                        self.read_code = '\t' + name + ' = new ' + true_type + '*[' + size + '];\n'
                        self.read_code += '\tfor(unsigned int i = 0; i < ' + size + '; i++) {\n'
                        self.read_code += '\t\t' + name + '[i] = new ' + true_type + '[' + size_2 +';\n'
                        self.read_code += '\t\treader.read(' + name + '[i], sizeof(' + true_type + '), ' + size_2 + ');\n'
                        self.read_code += '\t}\n'
                    else:
                        self.read_code = '\t' + name + ' = new ' + true_type + '*[' + size + '];\n'
                        self.read_code += '\tfor(unsigned int i = 0; i < ' + size + '; i++) {\n'
                        self.read_code += '\t\t' + name + '[i] = new ' + true_type + '[' + size_2 +'[i]];\n'
                        self.read_code += '\t\treader.read(' + name + '[i], sizeof(' + true_type + '), ' + size_2 + '[i]);\n'
                        self.read_code += '\t}\n'
                        
                    self.delete_code = '\tfor(unsigned int i = 0; i < ' + size + '; i++) {\n'
                    self.delete_code += '\t\tdelete[] ' + name + '[i];\n'
                    self.delete_code += '\t}\n'
                    self.delete_code += '\tdelete[] ' + name + ';\n'
                else:
                    self.declaration_type = '\t' + true_type + '* ' + name + ';\n'
                    self.read_code = '\t' + name + ' = new ' + true_type + '[' + size + '];\n'
                    self.read_code += '\treader.read(' + name + ', sizeof(' + true_type + '), ' + size + ');\n'
                    self.delete_code = '\tdelete[]' + name + ';\n'
            self.resolve_code = ''
        else:
            self.is_array = False
            self.declaration_type = '\t' + true_type + ' ' + name + ';\n'
            if context.is_bitfield_type(true_type):
                r_type = context.get_bitfield_storage_type(true_type)
                self.read_code = '\t' + r_type + ' ' + name + 'Data;\n'
                self.read_code += '\treader.read(&' + name + 'Data, sizeof(' + r_type + '), 1);\n'
                self.read_code += '\t' + name + ' = ' + name + 'Data;\n'
            else:
                self.read_code = '\treader.read(&' + name + ', sizeof(' + true_type + '), 1);\n'
            self.delete_code = ''
            self.resolve_code = ''


    def get_type_name(self):
        return self.type_name


class StringType:
    """
        Represents a string type.

        Attributes:
            is_array (bool) -- If true, the field is an array.
            fixed_array (bool) -- If true, the field is a fixed-size array.
            declaration_type (str) -- Field declaration line.
            read_code (str) -- Code for the loader function.
            resolve_code (str) -- Code for the pointer resolution function.
            delete_code (str) -- Code for the destructor.
    """

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
    """
        Represents a struct field type.

        Attributes:
            is_array (bool) -- If true, the field is an array.
            fixed_array (bool) -- If true, the field is a fixed-size array.
            declaration_type (str) -- Field declaration line.
            read_code (str) -- Code for the loader function.
            resolve_code (str) -- Code for the pointer resolution function.
            delete_code (str) -- Code for the destructor.
    """

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
    """
        Represents the C++ code for a struct or class field.
    """

    def __init__(self, context, element):
        self.name = process_name(element.getAttribute('name'))

        if element.firstChild is not None:
            self.description = element.firstChild.nodeValue.strip()
        else:
            self.description = None

        self.hasCondition = element.hasAttribute('cond')
        if self.hasCondition:
            self.condition = str(Expression(element.getAttribute('cond'), name_filter=process_name))

        f_type = element.getAttribute('type')
        if f_type == 'Region':
            breakpoint()

        if context.is_pointer_type(f_type):
            self.type = PointerType(element)
        elif context.is_basic_type(f_type) or context.is_enum_type(f_type) or context.is_bitfield_type(f_type):
            self.type = BasicType(context, element)
        elif context.is_string_type(f_type):
            self.type = StringType(element)
        elif context.is_compound_type(f_type):
            self.type = CompoundType(element)
        else:
            raise ValueError(f_type + ' is not a previously registered type!')

        if element.hasAttribute('onlyT'):
            self.only_type = element.getAttribute('onlyT')
        else:
            self.only_type = None

    def get_declaration(self):
        """
            Gets the field declaration.

            Returns:
                str
        """
        return self.type.declaration_type

    def get_constructor_code(self):
        """
            Gets the constructor code for this field.

            Returns:
                str
        """
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
        """
            Gets the destructor code for this field.

            Returns:
                str
        """
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
        """
            Gets the pointer resolution code for this field.

            Returns:
                str
        """
        out = ''
        out += self.type.resolve_code
        return out

    def get_type(self):
        return self.type.get_type_name()
