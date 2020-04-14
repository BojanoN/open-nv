#include "esm.hpp"
#include "reader.hpp"
#include <cassert>

namespace ESM {

ESM::ESM(std::string& path)
    : reader(path)
    , records() {};

ESM::~ESM() {};
};
