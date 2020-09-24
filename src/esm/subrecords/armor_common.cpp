#include "armor_common.hpp"

namespace ESM {

void ArmorAttributes::load(ESMReader& reader, ArmorAttributes& attrs)
{
    if (reader.getCurrentSubrecord().dataSize == 4) {
        reader.readSubrecord(*reinterpret_cast<ArmorAttributes::ArmorAttributesSmall*>(&attrs));
    } else {
        reader.readSubrecord(attrs);
    }
}

}; // namespace ESM