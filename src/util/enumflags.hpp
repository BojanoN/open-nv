#pragma once
#include <type_traits>

template <typename Enum>
struct EnableBitwiseOperators {
    static const bool enable = false;
};
template <typename Enum>
typename std::enable_if<EnableBitwiseOperators<Enum>::enable, Enum>::type
operator|=(Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
};

template <typename Enum>
typename std::enable_if<EnableBitwiseOperators<Enum>::enable, Enum>::type
operator&=(Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
};

template <typename Enum>
typename std::enable_if<EnableBitwiseOperators<Enum>::enable, bool>::type operator|(Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<bool>(
        static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
};

template <typename Enum>
typename std::enable_if<EnableBitwiseOperators<Enum>::enable, bool>::type
operator&(Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<bool>(
        static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
};

#define ENABLE_BITWISE_OPERATORS(x)      \
    template <>                          \
    struct EnableBitwiseOperators<x> {   \
        static const bool enable = true; \
    }
