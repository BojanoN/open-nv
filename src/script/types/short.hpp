#pragma once

#include "base.hpp"

namespace Script {
class Short : public Object {
public:
    Short(int16_t val)
        : Object(Type::Reference)
        , value(val) {};
    int16_t value;
};
}
