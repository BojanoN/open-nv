#pragma once
#include "record.hpp"
#include "structs.hpp"

namespace ESM {

enum class VoiceTypeFlags : uint8_t {
	ALLOW_DEFAULT_DIALOG = 0x01,
	FEMALE = 0x02
};

struct VoiceType : public Record {

    std::string    editorId;
    VoiceTypeFlags flags;

    VoiceType(ESMReader& reader);
};
};