import string

def process_name(name):
    """
        Converts an attribute name from a string to a C++ compatible camelCase name.
        Example: "Num Of Strips" -> "numOfStrips"

        Parameters:
            name (str) -- Name to convert.

        Returns:
            (str) -- Converted name.
    """
    if ':' in name:
        name = name.replace(':', '')
    if name == '#ARG#':
        return '0'
    parts = name.split(' ')
    parts[0] = parts[0].lower()
    return ''.join(parts)

def process_array_size(array_size):
    """
        Converts an array size expression from a string into a C++ compatible expression.
        Example: "Num Faces * Num Pixels" -> "numFaces * numPixels"

        Parameters:
            array_size (str) -- Expression to convert.

        Returns:
            (str) -- Converted expression.
        
    """
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
    """
        Converts an enum name into a C++ compatible enum name.
        Example: "Specular Strength" -> SPECULAR_STRENGTH

        Parameters:
            name (str) -- Name to convert.

        Returns:
            (str) -- Converted name.
    """
    if ' ' in name:
        new_name = name.replace(' ', '_')
    elif '_' not in name:
        for uppercase_char in string.ascii_uppercase:
            new_name = name.replace(uppercase_char, '_' + uppercase_char)
    else:
        new_name = name
    return new_name.upper()