#pragma once
#include "logc/log.h"
#include "sds/sds.h"
#include "util/container.h"
#include "util/reference.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint32_t type;
    uint8_t  Type[4];
    uint16_t DataSize;

} Subrecord;

typedef struct {
    uint8_t  Type[4];
    uint32_t DataSize;
    uint32_t Flags;
    uint32_t ID;
    uint32_t CreationKitRevision;
    uint16_t FormVersion;

    /* ignoring this for now */
    uint16_t Unknown;
    // TODO: container of subrecords
    Subrecord* subrecords;
} Record;

Record* recordnew(FILE* f);
void    recordfree(Record* record);

typedef enum {
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
} RecordFlags;

/*
 * Zlib compressed records
 */
typedef struct {
    uint32_t DecompSize;
} CompressedData;

typedef struct {
    struct {
        uint32_t key;
        Record*  value;
    } * records;
} Esm;

Esm* esmnew(const sds path);
void esmfree(Esm* esm);
