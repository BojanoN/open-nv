#pragma once
#include "types.hpp"
#include "util/enumflags.hpp"

namespace ESM {

class ESMReader;

enum class RecordFlags : uint32_t {
    MASTER    = 1,
    FORM_INIT = (1 << 4),
    DELETED   = (1 << 5),
    /*
     * Has multiple meanings depending on the record type:
     * Border Region / Has Tree LOD / Constant / Hidden From Local Map
     */
    BORDER        = (1 << 6),
    TURN_OFF_FIRE = (1 << 7),
    INACCESSIBLE  = (1 << 8),
    /*
     * Casts shadows / On Local Map / Motion Blur
     */
    CASTS_SHADOW = (1 << 9),
    /*
     * Quest item / Persistent reference
     */
    QUEST_ITEM      = (1 << 10),
    INIT_DISABLED   = (1 << 11),
    IGNORED         = (1 << 12),
    NO_VOICE_FILTER = (1 << 13),
    CANT_SAVE       = (1 << 14),
    VIS_DISTANT     = (1 << 15),
    /*
     * Random Anim Start / High Priority LOD
     */
    RANDOM_ANIM = (1 << 16),
    /*
     * Dangerous / Off limits (Interior cell) / Radio Station (Talking Activator)
     */
    DANGER     = (1 << 17),
    COMPRESSED = (1 << 18),
    /*
     * Can't wait / Platform Specific Texture / Dead
     */
    NO_WAIT      = (1 << 19),
    DESTRUCTIBLE = (1 << 24),
    /*
     * Obstacle / No AI Acquire
     */
    OBSTACLE       = (1 << 25),
    NAVMESH_FILTER = (1 << 26),
    NAVMESH_BBOX   = (1 << 27),
    /*
     * Non-Pipboy / Reflected by Auto Water
     */
    NON_PIPBOY = (1 << 28),
    /*
     * Child Can Use / Refracted by Auto Water
     */
    CHILD_USABLE   = (1 << 29),
    NAVMESH_GROUND = (1 << 30)
};

struct RecordHeader {
    uint32_t    type;
    uint32_t    dataSize;
    RecordFlags flags;
    formid      id;
    uint32_t    creationKitRevision;
    uint16_t    formVersion;
    /* ignoring this for now */
    uint16_t unknown;
};

struct GroupHeader {
    uint32_t type;
    uint32_t groupSize;
    uint32_t label;
    int32_t  groupType;
    uint16_t stamp;
    uint8_t  unknown[6];
};

#pragma pack(1)
struct SubrecordHeader {
    uint32_t type;
    uint16_t dataSize;
};

struct CompressedDataHeader {
    uint32_t decompSize;
};
#pragma options align = reset

};
ENABLE_BITWISE_OPERATORS(ESM::RecordFlags);
