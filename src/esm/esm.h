#pragma once
#include "init.h"
#include "logc/log.h"
#include "sds/sds.h"
#include "util/container.h"
#include "util/reference.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct __attribute__((packed)) {
    uint8_t  Type[4];
    uint16_t DataSize;
} Subrecord;

DEFINE_OBJECT_TYPEDEFS(Subrecord);
DECLARE_MAP_INITIALIZERS(Subrecord);

#define log_subrecord(subrec) (log_debug("Subrecord %.4s: Size: %hi", (subrec)->base.Type, (subrec)->base.DataSize));
#define log_subrecord_new(subrec)  (log_debug("Subrecord %.4s: Size: %hi", subrec->Type, subrec->DataSize));

typedef uint32_t formid;

typedef struct __attribute__((packed)) {
    uint8_t  Type[4];
    uint32_t DataSize;
    uint32_t Flags;
    uint32_t ID;
    uint32_t CreationKitRevision;
    uint16_t FormVersion;
    /* ignoring this for now */
    uint16_t Unknown;
} Record;

#define RECORD_SIZE     (sizeof(Record))
#define log_record(rec) (log_info("Record %.4s: Size: %u, ID: %u", (rec)->Type, (rec)->DataSize, (rec)->ID));

Record* recordnew(FILE* f, sds type);
void    recordfree(Record* record);

/*
 * Record constructor and destructor typedef.
 */
DEFINE_OBJECT_TYPEDEFS(Record);
DECLARE_MAP_INITIALIZERS(Record);

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

typedef struct {
    uint8_t  Type[4];
    uint32_t GroupSize;
    uint8_t  Label[4];
    uint32_t GroupType;
    uint16_t Datestamp;
    uint8_t  Unknown[6];
    /*
   * Data size = GroupSize - 24;
   */
} Group;

/*
 * Enum for group types.
 */
typedef enum {
    /*
     * Contains records of type given in label.
     */
    TOP_LEVEL,
    /*
     * Contains ROAD or CELL records whose parent is a WRLD record.
     * Parent record ID is given in the label.
     */
    WORLD_CHILDREN,
    /*
     * Label contains the cell block number.
     */
    INTERIOR_CELL_BLOCK,
    /*
     * Label contains the cell sub-block number.
     */
    INTERIOR_CELL_SUBBLOCK,
    /*
     * Label contains cell coords stored as uint8_t, (y, x)
     */
    EXTERIOR_CELL_BLOCK,
    /*
     * Label contains cell coords stored as uint8_t, (y, x)
     */
    EXTERIOR_CELL_SUBBLOCK,
    /*
     * Contains only REFR, ACRE, PGRE, PMIS or ACHR records
     * that are children of the given CELL record.
     * Parent record ID is given in the label.
     */
    CELL_CHILDREN,
    /*
     * Contains INFO records that are children of the given DIAL record.
     * Parent record ID is given in the label.
     */
    TOPIC_CHILDREN,
    /*
     * The group must contain only REFR, ACRE, PGRE, PMIS or ACHR
     * records that are children of the given CELL record.
     * Parent record ID is given in the label.
     */
    CELL_PERS_CHILDREN,
    /*
     * Same as above.
     */
    CELL_TEMP_CHILDREN,
    /*
     * The group must contain only REFR records that are children of the given CELL record.
     * Parent record ID is given in the label.
     */
    CELL_VISIBLE_DISTANT_CHILDREN
} GroupTypes;

#define GROUP_SIZE (sizeof(Group))
#define GROUP_TYPE "GRUP"

Group* groupnew(FILE* esm_file);
void   groupfree(Group* group);

/*
 * Group constructor and destructor typedef.
 */
DEFINE_OBJECT_TYPEDEFS(Group);
DECLARE_MAP_INITIALIZERS(Group);

#define log_group(group) log_info("Group %.4s: Size: %u, Label: %.4s", (group)->base.Type, (group)->base.GroupSize, (group)->base.Label)

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

    Group** groups;
} Esm;

Esm* esmnew(const sds path);
void esmfree(Esm* esm);
