#pragma once
#include "record.hpp"
#include "structs.hpp"
#include <variant>

namespace ESM {
class ESMReader;

enum class NoteType : uint8_t {
    Sound,
    Text,
    Image,
    Voice
};

struct Note : public Record {
    ObjectBounds                      objectBounds;
    std::string                       name;
    ModelData                         modelData;
    std::string                       largeIconFilename;
    std::string                       smallIconFilename;
    formid                            pickUpSound;
    formid                            dropSound;
    NoteType                          type;
    formid                            quest; // QUST
    std::string                       texture;
    std::variant<std::string, formid> textOrTopic;
    formid                            soundOrActor; // SOUN, NPC_ or CREA

    Note(ESMReader& reader);
};
}
