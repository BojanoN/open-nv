#include "data_file.hpp"

class HEDR : public SubRecord {
    HEDR(std::ifstream& stream) : SubRecord(stream)
    {
    }
};
