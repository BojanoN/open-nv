#pragma once

#include "esm.h"
#include "subrecords.h"

#define FILL_BASE_RECORD_INFO(header, record) \
    record->base.ID    = header.ID;           \
    record->base.Flags = header.Flags;        \
    memcpy(record->base.Type, header.Type, 4)

#define SUBRECORD_WITH_HEADER_READ(INNER_MACRO, ...)         \
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file); \
    INNER_MACRO(__VA_ARGS__);

#define OPTIONAL_CSTRING_SUBRECORD(subrecordName, value, subheader, esm_file,   \
    logging_name)                                                               \
    if (strncmp(subheader.Type, (subrecordName), 4) == 0) {                     \
        (value) = init_cstring_subrecord(esm_file, &(subheader), logging_name); \
    } else {                                                                    \
        (value) = NULL;                                                         \
        fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);                    \
    }

#define CSTRING_SUBRECORD(subrecordName, value, subheader, esm_file, \
    logging_name)                                                    \
    assert(strncmp((subheader).Type, (subrecordName), 4) == 0);      \
    (value) = init_cstring_subrecord(esm_file, &(subheader), logging_name)

#define GENERIC_SUBRECORD(subrecordName, type, value, subheader, esm_file) \
    assert(strncmp((subheader).Type, (subrecordName), 4) == 0);            \
    fread(&(value), sizeof(type), 1, esm_file);                            \
    log_subrecord(&subheader)

#define OPTIONAL_MAIN_SUBRECORD(subrecordName, type, value, subheader, \
    esm_file, logging_format_string)                                   \
    if (strncmp(subheader.Type, (subrecordName), 4) == 0) {            \
        fread(&(value), sizeof(type), 1, esm_file);                    \
        log_subrecord(&subheader);                                     \
        log_debug(logging_format_string, value);                       \
    } else {                                                           \
        fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);           \
    }

#define OPTIONAL_STRUCT_SUBRECORD(subrecordName, type, value, subheader, \
    esm_file)                                                            \
    if (strncmp(subheader.Type, (subrecordName), 4) == 0) {              \
        fread(&(value), sizeof(type), 1, esm_file);                      \
        log_subrecord(&subheader);                                       \
        log_##type(&value);                                              \
    } else {                                                             \
        fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);             \
    }

#define MAIN_SUBRECORD(subrecordName, type, value, subhreader, esm_file, logging_format_string) \
    GENERIC_SUBRECORD(subrecordName, type, value, subheader, esm_file);                         \
    log_debug(logging_format_string, value)

#define STRUCT_SUBRECORD(subrecordName, type, value, subhreader, esm_file) \
    GENERIC_SUBRECORD(subrecordName, type, value, subheader, esm_file);    \
    log_##type(&value)

#define GENERIC_SUBRECORD(subrecordName, type, value, subheader, esm_file) \
    assert(strncmp((subheader).Type, (subrecordName), 4) == 0);            \
    fread(&(value), sizeof(type), 1, esm_file);                            \
    log_subrecord(&subheader)

#define MAIN_SUBRECORD_COLLECTION(subrecordName, type, array, subheader, \
    esm_file, logging_format_string)                                     \
    while (strncmp((subheader).Type, (subrecordName), 4) == 0) {         \
        type tmp;                                                        \
        assert(strncmp((subheader).Type, (subrecordName), 4) == 0);      \
        fread(&(tmp), sizeof(type), 1, esm_file);                        \
        log_subrecord(&subheader);                                       \
        arrput((array), tmp);                                            \
        log_debug(logging_format_string, tmp);                           \
        fread(&(subheader), sizeof(SubrecordHeader), 1, esm_file);       \
    }                                                                    \
    fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR)

#define STRUCT_SUBRECORD_COLLECTION(subrecordName, type, array, subheader, \
    esm_file)                                                              \
    while (strncmp((subheader).Type, (subrecordName), 4) == 0) {           \
        type tmp;                                                          \
        assert(strncmp((subheader).Type, (subrecordName), 4) == 0);        \
        fread(&(tmp), sizeof(type), 1, esm_file);                          \
        log_subrecord(&subheader);                                         \
        arrput((array), tmp);                                              \
        log_##type(&(tmp));                                                \
        fread(&(subheader), sizeof(SubrecordHeader), 1, esm_file);         \
    }                                                                      \
    fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR)

#define ARRAY_SUBRECORD(subrecordName, type, value, subheader, esm_file, \
    logging_format, logging_name)                                        \
    assert(strncmp(subheader.Type, subrecordName, 4) == 0);              \
    log_subrecord(&subheader);                                           \
    MALLOC_N_WARN_BARE(type, subheader.DataSize, value);                 \
    {                                                                    \
        int length  = subheader.DataSize / sizeof(type);                 \
        readCounter = fread(value, sizeof(type), length, esm_file);      \
        assert(readCounter == length);                                   \
    }

#define OPTIONAL_MAIN_SUBRECORD_COLLECTION(subrecordName, type, array,   \
    subheader, esm_file, logging_format_string)                          \
    if (strncmp(subheader.Type, (subrecordName), 4) == 0) {              \
        MAIN_SUBRECORD_COLLECTION(subrecordName, type, array, subheader, \
            esm_file, logging_format_string);                            \
    } else {                                                             \
        fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);             \
    }

#define OPTIONAL_SUBRECORD_COLLECTION(subrecordName, type, array, subheader, \
    esm_file)                                                                \
    if (strncmp(subheader.Type, (subrecordName), 4) == 0) {                  \
        STRUCT_SUBRECORD_COLLECTION(subrecordName, type, array, subheader,   \
            esm_file);                                                       \
    } else {                                                                 \
        fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);                 \
    }

#define FIXED_LENGTH_ARRAY_SUBRECORD(subrecordName, type, length, value, \
    subheader, esm_file, logging_format,                                 \
    logging_name)                                                        \
    assert(strncmp(subheader.Type, subrecordName, 4) == 0);              \
    log_subrecord(&subheader);                                           \
    readCounter = fread(&(value), sizeof(type),                          \
        length, esm_file);                                               \
    assert(readCounter == length)

#define MARKER_SUBRECORD(subrecordName, subheader)          \
    assert(strncmp(subheader.Type, subrecordName, 4) == 0); \
    assert(subheader.DataSize == 0)

#define DUMMY_SUBRECORD(subrecordName, subheader)           \
    assert(strncmp(subheader.Type, subrecordName, 4) == 0); \
    log_subrecord(&subheader);                              \
    fseek(esm_file, subheader.DataSize, SEEK_CUR)
/*
    TODO: multiple type collection?
*/

typedef struct {
    Record base;
    HEDR*  hedr;
    CNAM*  cnam;
} TES4Record;

/*
    Game setting
*/
typedef struct {
    Record base;
    sds    editorID;
    union {
        int32_t intValue;
        float   floatValue;
        sds     stringValue;
    } value;
} GMSTRecord;

/*
    Texture set
*/
typedef struct {
    Record       base;
    sds          editorID;
    ObjectBounds objectBounds;
    sds          baseImage_transparency;
    sds          normalMap_specular;
    sds          environmentMapMask;
    sds          glowMap;
    sds          parallaxMap;
    sds          environmentMap;
    DecalData    decalData;
    uint16_t     flags;
} TXSTRecord;

/*
 * Menu icon
 */
typedef struct {
    Record base;
    sds    editorID;
    sds    largeIconFilename;
    sds    smallIconFilename;
} MICNRecord;

/*
  Global variable
*/
typedef struct {
    Record  base;
    sds     editorID;
    uint8_t type;
    union {
        uint16_t shortValue;
        uint32_t longValue;
        float    floatValue;
    } value;
} GLOBRecord;

/*
 * Class
 */

typedef struct {
    Record          base;
    sds             editorID;
    sds             name;
    sds             description;
    ClassData       data;
    ClassAttributes attributes;
} CLASRecord;

typedef enum {
    BARTER_SKILL,
    BIG_GUNS_SKILL,
    ENERGY_WEAP_SKILL,
    EXPLOSIVES_SKILL,
    LOCKPICK_SKILL,
    MEDICINE_SKILL,
    MELEE_WEAP_SKILL,
    REPAIR_SKILL,
    SCIENCE_SKILL,
    GUNS_SKILL,
    SNEAK_SKILL,
    SPEECH_SKILL,
    SURVIVAL_SKILL,
    UNARMED_SKILL,
    NONE = -1
} Skill;

/*
 * Faction
 */
typedef struct {
    Record         base;
    sds            editorID;
    sds            name;
    uint32_t       relationsLength;
    FactionRaceEx* relations;
    FactionData    data;
    float          unused;
    FactionRank*   ranks;
    formid         reputation; //REPU
} FACTRecord;

/*
 * Head part.
 */
typedef struct {
    Record              base;
    sds                 editorID;
    sds                 name;
    uint8_t             flag;
    ModelDataSubrecord* modelData;
    /*
     * FormID of another HDPT record
     */
    formid* extraParts;
} HDPTRecord;

/*
 * Hair.
 */
typedef struct {
    Record              base;
    sds                 editorID;
    sds                 name;
    ModelDataSubrecord* modelData;
    sds                 texture;
    uint8_t             flag;
} HAIRRecord;

/*
 * Eyes.
 */
typedef struct {
    Record base;
    sds    editorID;
    sds    name;
    /*
     * This field is optional.
     */
    sds     texture;
    uint8_t flag;
} EYESRecord;

/*
 * Race struct.
 */
typedef struct {
    Record         base;
    sds            editorID;
    sds            name;
    sds            description;
    FactionRaceEx* relations;
    RaceData       raceData;
    /*
     * FormID of a RACE record
     */
    formid older;
    formid younger;

    RaceVoices           voices;
    RaceDefaultHairStyle defaultHair;
    RaceDefaultHairColor defaultHairColor;

    float   faceGenMainClamp;
    float   faceGenFaceClamp;
    uint8_t unknownAttr[2];

    ModelPart* maleHeadParts;
    ModelPart* femaleHeadParts;
    ModelPart* maleBodyParts;
    ModelPart* femaleBodyParts;

    formid* hair;
    formid* eyes;
    /*
     * FaceGen data
     */
    uint8_t* maleFaceGenGeomSymm;
    uint8_t* maleFaceGenGeomAsymm;
    uint8_t* maleFaceGenTexSymm;
    // SNAM, but use is unknown
    uint8_t* femaleFaceGenGeomSymm;
    uint8_t* femaleFaceGenGeomAsymm;
    uint8_t* femaleFaceGenTexSymm;
    // SNAM ?
} RACERecord;

/*
    Sound
*/
typedef struct {
    Record       base;
    sds          editorID;
    ObjectBounds objectBounds;
    sds          soundFilename;
    uint8_t      randomChangePercentage;
    SoundData    soundData;
} SOUNRecord;

/*
    Part of ASPC
*/
typedef enum {
    NONE_ENVIRONMENT_TYPE,
    DEFAULT,
    GENERIC,
    PADDED_CELL,
    ROOM,
    BATHROOM,
    LIVINGROOM,
    STONE_ROOM,
    AUDITORIUM,
    CONCERTHALL,
    CAVE,
    ARENA,
    HANGAR,
    CARPETED_HALLWAY,
    HALLWAY,
    STONE_CORRIDOR,
    ALLEY,
    FOREST,
    CITY,
    MOUNTAINS,
    QUARRY,
    PLAIN,
    PARKINGLOT,
    SEWERPIPE,
    UNDERWATER,
    SMALL_ROOM,
    MEDIUM_ROOM,
    LARGE_ROOM,
    MEDIUM_HALL,
    LARGE_HALL,
    PLATE
} EnvironmentType;

/*
    Part of ASPC
*/
typedef enum {
    NO  = 0,
    YES = 1
} IsInterior;

/*
    Acoustic space
*/
typedef struct {
    Record       base;
    sds          editorID;
    ObjectBounds objectBounds;
    formid       dawn_default; //SOUN
    formid       afternoon; //SOUN
    formid       dusk; //SOUN
    formid       night; //SOUN
    formid       walla; //SOUN
    uint32_t     wallaTriggerCount;
    formid       regionSound; //REGN
    /*
        see EnvironmentType enum
    */
    uint32_t environmentType;
    /*
        see IsInterior enum
    */
    uint32_t isInterior;
} ASPCRecord;

/*
    Magic effect
*/
typedef struct {
    Record              base;
    sds                 editorID;
    sds                 name;
    sds                 description;
    sds                 largeIconFilename;
    sds                 smallIconFilename;
    ModelDataSubrecord* modelData;
    MagicEffectData     magicEffectData;
} MGEFRecord;

/*
    Script
*/
typedef struct {
    Record           base;
    sds              editorID;
    ScriptHeader     scriptHeader;
    uint8_t*         compiledSource;
    char*            scriptSource;
    LocalVariable*   localVariables;
    ScriptReference* references;
} SCPTRecord;

/*
 * Land texture.
 */

typedef struct {
    Record base;
    sds    editorID;
    sds    largeIcon;
    sds    smallIcon;
    // TXST
    formid    texture;
    HavokData havok;
    uint8_t   textureSpecularExponent;
    // GRAS
    formid* grass;

} LTEXRecord;

/*
 * Enchantment.
 */

typedef struct {
    Record    base;
    sds       editorID;
    sds       name;
    ENCH_enit effectData;

} ENCHRecord;
