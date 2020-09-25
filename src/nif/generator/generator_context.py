from expression import Expression

class GeneratorContext:
    """
    Context for the code generator.
    Keeps information about available types.
    """

    generic_type_marker = '#T#'

    def __init__(self, version, basic_types, string_types, pointer_types):
        """
        Parameters:
            version (Version) -- The version.
            basic_types (dict) -- Dictionary which maps basic types to C++ representations.
            string_types (list) -- List of string types.
            pointer_types (list) -- List of pointer types.
        """
        self.version = version
        self.enum_types = set()
        self.bitfield_types = dict()
        self.compound_types = set()
        self.basic_types = basic_types
        self.string_types = string_types
        self.pointer_types = pointer_types

    def add_enum_type(self, enum_type):
        """
        Register the type as an enum type.

        Parameters:
            enum_type (str) -- The type name.
        """
        self.enum_types.add(enum_type)

    def is_enum_type(self, query_type):
        """
        Returns true if the type is a registered enum type.

        Returns:
            bool
        """
        return query_type in self.enum_types

    def add_bitfield_type(self, bitfield_type, storage_type):
        """
        Register the type as a btfield type.

        Parameters:
            bitfield_type (str) -- The type name.
            storage_type (str) -- The name of the bitfield storage type.
        """
        self.bitfield_types[bitfield_type] = storage_type

    def get_bitfield_storage_type(self, bitfield_type):
        """
        Returns the bitfield storage type.

        Parameters:
            bitfield_type (str) -- The type name.
        Returns:
            str -- The storage type.
        """
        return self.bitfield_types[bitfield_type]

    def is_bitfield_type(self, query_type):
        """
        Returns true if the type is a registered bitfield type.

        Returns:
            bool
        """
        return query_type in self.bitfield_types

    def is_basic_type(self, query_type):
        """
        Returns true if the type is a basic type.

        Returns:
            bool
        """
        return query_type in self.basic_types

    def get_basic_type(self, query_type):
        """
        Returns the C++ type representation of the given basic type.

        Returns:
            str -- The C++ representation of the basic type.
        """
        return self.basic_types[query_type]

    def is_string_type(self, query_type):
        """
        Returns true if the type is a string type.

        Returns:
            bool
        """
        return query_type in self.string_types

    def add_compound_type(self, compound_type):
        """"
        Register the type as a compound type.

        Parameters:
            compound_type (str) -- The type name.
        """
        self.compound_types.add(compound_type)

    def is_compound_type(self, query_type):
        """
        Returns true if the type is a compound type.

        Returns:
            bool
        """
        return query_type in self.compound_types or query_type == self.generic_type_marker

    def is_pointer_type(self, query_type):
        """
        Returns true if the type is a pointer type.

        Returns:
            bool
        """
        return query_type in self.pointer_types

    def is_correct_version(self, field):
        """
        Returns true if the field is available in this version.

        Returns:
            bool
        """
        if field.hasAttribute('ver1'):
            if self.version.num < self.version.get_value(field.getAttribute('ver1')):
                return False
        if field.hasAttribute('ver2'):
            if self.version.num > self.version.get_value(field.getAttribute('ver2')):
                return False
        if field.hasAttribute('vercond'):
            if not bool(Expression(field.getAttribute('vercond')).eval(self.version)):
                return False
        return True


def get_basic_types():
    """
        Returns a fixed dictionary of basic type mappings.
    """
    return {
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

def get_string_types():
    """
        Returns a fixed list of string types.
    """
    return ['string', 'SizedString', 'SizedString16']

def get_pointer_types():
    """
        Returns a fixed list of pointer types.
    """
    return ['Ptr', 'Ref']