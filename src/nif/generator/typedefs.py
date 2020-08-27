def get_typedefs():
    """
        Gets the typedefs header code.

        The typedefs headers code includes all nonstandard types.

        In case of Fallout NV, only boolean size needs to be enforced.
    """
    return """
    #pragma once
    #include <cstdint>
    typedef uint8_t nif_bool_t;
    """