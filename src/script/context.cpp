#include "context.hpp"
#include "types/base.hpp"

namespace Script {
Context::~Context()
{
    for (auto& e : this->variables) {
        delete e.second;
    }
}

}
