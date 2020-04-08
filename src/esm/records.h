#pragma once

#include "esm.h"
#include "subrecords.h"

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
    Record             base;
    sds                editorID;
    sds                name;
    uint8_t            flag;
    ModelDataSubrecord modelData;
    /*
     * FormID of another HDPT record
     */
    formid extraParts;
} HDPTRecord;
