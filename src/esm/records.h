#pragma once

#include "esm.h"
#include "subrecords.h"

#define FILL_BASE_RECORD_INFO(header, record) \
    record->base.ID    = header.ID;           \
    record->base.Flags = header.Flags;        \
    memcpy(record->base.Type, header.Type, 4)

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
    BARTER,
    BIG_GUNS,
    ENERGY_WEAP,
    EXPLOSIVES,
    LOCKPICK,
    MEDICINE,
    MELEE_WEAP,
    REPAIR,
    SCIENCE,
    GUNS,
    SNEAK,
    SPEECH,
    SURVIVAL,
    UNARMED,
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
    Record base;
    sds editorID;
    OBNDSubrecord objectBounds;
    sds soundFilename;
    uint8_t randomChangePercentage;
    SoundData soundData;
} SOUNRecord;