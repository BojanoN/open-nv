#pragma once

#include "esm.h"
#include "subrecords.h"

#define FILL_BASE_RECORD_INFO(header, record) \
    record->base.ID    = header.ID;           \
    record->base.Flags = header.Flags;        \
    memcpy(record->base.Type, header.Type, 4)

#define OPTIONAL_CSTRING_RECORD(subrecordName, value, subheader,                \
    esm_file, logging_name)                                                     \
    fread(&(subheader), sizeof(SubrecordHeader), 1, esm_file);                  \
    if (strncmp(subheader.Type, (subrecordName), 4) == 0) {                     \
        (value) = init_cstring_subrecord(esm_file, &(subheader), logging_name); \
    } else {                                                                    \
        (value) = NULL;                                                         \
        fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);                    \
    }

#define CSTRING_RECORD(subrecordName, value, subheader, esm_file, \
    logging_name)                                                 \
    fread(&(subheader), sizeof(SubrecordHeader), 1, esm_file);    \
    assert(strncmp((subheader).Type, (subrecordName), 4) == 0);   \
    (value) = init_cstring_subrecord(esm_file, &(subheader), logging_name)

#define GENERIC_RECORD(subrecordName, type, value, subheader, esm_file) \
    fread(&(subheader), sizeof(SubrecordHeader), 1, esm_file);          \
    assert(strncmp((subheader).Type, (subrecordName), 4) == 0);         \
    fread(&(value), sizeof(type), 1, esm_file);                         \
    log_subrecord(&subheader);

#define OPTIONAL_GENERIC_RECORD(subrecordName, type, value, subheader, esm_file) \
    fread(&(subheader), sizeof(SubrecordHeader), 1, esm_file);                   \
    if (strncmp(subheader.Type, (subrecordName), 4) == 0) {                      \
        fread(&(value), sizeof(type), 1, esm_file);                              \
        log_subrecord(&subheader);                                               \
    } else {                                                                     \
        fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);                     \
    }

#define GENERIC_RECORD_COLLECTION(subrecordName, type, array, subheader, esm_file) \
    fread(&(subheader), sizeof(SubrecordHeader), 1, esm_file);                     \
    while (strncmp((subheader).Type, (subrecordName), 4) == 0) {                   \
        type tmp;                                                                  \
        assert(strncmp((subheader).Type, (subrecordName), 4) == 0);                \
        fread(&(tmp), sizeof(type), 1, esm_file);                                  \
        log_subrecord(&subheader);                                                 \
        arrput((array), tmp);                                                      \
        fread(&(subheader), sizeof(SubrecordHeader), 1, esm_file);                 \
    }                                                                              \
    fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR)

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
    sds    editorId;
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
    Record        base;
    sds           editorId;
    OBNDSubrecord objectBounds;
    sds           baseImage_transparency;
    sds           normalMap_specular;
    sds           environmentMapMask;
    sds           glowMap;
    sds           parallaxMap;
    sds           environmentMap;
    DODTSubrecord decalData;
    uint16_t      flags;
} TXSTRecord;

/*
 * Menu icon
 */
typedef struct {
    Record base;
    sds    editorID;
    sds    largeIconFilename;
    sds    smallIconFilename;
} MICN;

/*
  Global variable
*/
typedef struct {
    Record  base;
    sds     editorId;
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
    Record        base;
    sds           editorID;
    sds           fullName;
    sds           description;
    DATASubrecord data;
    ATTRSubrecord attr;
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
    Record               base;
    sds                  editorId;
    sds                  name;
    uint32_t             relationsLength;
    XNAMSubrecord*       relations;
    FACT_DATASubrecord   data;
    float                unused;
    FACT_RankSubrecords* rank;
    formid               reputation;
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
    formid extraParts;
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
    Record            base;
    sds               editorID;
    sds               name;
    sds               description;
    XNAMSubrecord*    relations;
    RaceDataSubrecord raceData;
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
    Record        base;
    sds           editorID;
    OBNDSubrecord objectBounds;
    sds           soundFilename;
    uint8_t       randomChangePercentage;
    SoundData     soundData;
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
    Record        base;
    sds           editorID;
    OBNDSubrecord objectBounds;
    formid        dawn_default; //SOUN
    formid        afternoon; //SOUN
    formid        dusk; //SOUN
    formid        night; //SOUN
    formid        walla; //SOUN
    uint32_t      wallaTriggerCount;
    formid        regionSound; //REGN
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
