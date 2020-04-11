#include "records.h"
#include "util/memutils.h"

DECLARE_FUNCTION_MAPS(Record);

FETCH_CONSTRUCTOR_MAP(Subrecord);
FETCH_DESTRUCTOR_MAP(Subrecord);

Record* init_TES4(FILE* esm_file)
{
    MALLOC_WARN(TES4Record, ret);
    RecordHeader hdr;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, ret);

    SubrecordConstructor* func  = GET_CONSTRUCTOR(Subrecord, "HEDR");
    HEDR*                 tmp_h = (HEDR*)func(esm_file);

    if (tmp_h == NULL) {
        return NULL;
    }
    ret->hedr = tmp_h;

    func        = GET_CONSTRUCTOR(Subrecord, "CNAM");
    CNAM* tmp_c = (CNAM*)func(esm_file);

    if (tmp_c == NULL) {
        return NULL;
    }
    ret->cnam = tmp_c;

    log_record(&hdr);

    return (Record*)ret;
}

Record* init_GMST(FILE* esm_file)
{
    MALLOC_WARN(GMSTRecord, record);
    RecordHeader hdr;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);
    log_record(&hdr);

    //EDID
    SubrecordHeader  subrec;
    SubrecordHeader* subrecordHead = &subrec;
    fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);

    record->editorId = init_cstring_subrecord(esm_file, subrecordHead, "Editor ID");

    //DATA
    fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);

    size_t dataSize = subrecordHead->DataSize;

    log_subrecord(subrecordHead);

    switch ((record->editorId)[0]) {
    case 's': {
        char* cstring = malloc(dataSize);
        fread(cstring, dataSize, 1, esm_file);
        record->value.stringValue = sdsnewlen(cstring, dataSize);
        free(cstring);
        log_debug("Game setting string value: %s", record->value.stringValue);
        break;
    }
    case 'f':
        fread(&(record->value.floatValue), sizeof(float), 1, esm_file);
        log_debug("Game setting float value: %f", record->value.floatValue);
        break;
    default:
        fread(&(record->value.intValue), sizeof(int32_t), 1, esm_file);
        log_debug("Game setting integer value: %d", record->value.intValue);
        break;
    }

    return (Record*)record;
}

//TX00-TX05
static const char* optionalCstringSubrecordTypes[] = { "TX00", "TX01", "TX02",
    "TX03", "TX04", "TX05" };

static const char* optionalNames[] = {
    "Base image/Transparency",
    "Normal map/Specular",
    "Environment map mask",
    "Glow map",
    "Parallax map",
    "Environment map"
};

Record* init_TXST(FILE* esm_file)
{
    MALLOC_WARN(TXSTRecord, record);
    RecordHeader hdr;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    //EDID
    SubrecordHeader  subrecord;
    SubrecordHeader* subrecordHead = &subrecord;
    fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);

    record->editorId = init_cstring_subrecord(esm_file, subrecordHead, "Editor ID");
    fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);

    SubrecordConstructor* constructor = NULL;
    //OBND
    if (strncmp(subrecordHead->Type, "OBND", 4) == 0) {
        constructor                 = GET_CONSTRUCTOR(Subrecord, "OBND");
        OBNDSubrecord* objectBounds = (OBNDSubrecord*)constructor(esm_file);
        record->objectBounds        = *objectBounds;
        free(objectBounds);
        log_subrecord(subrecordHead);
        fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);
    }

    for (int i = 0; i < 6; i++) {
        if (strncmp(subrecordHead->Type, optionalCstringSubrecordTypes[i], 4) == 0) {
            sds tmp = init_cstring_subrecord(esm_file, subrecordHead, optionalNames[i]);
            switch (i) {
            case (0): {
                record->baseImage_transparency = tmp;
                break;
            }

            case (1): {
                record->normalMap_specular = tmp;
                break;
            }
            case (2): {
                record->environmentMapMask = tmp;
                break;
            }
            case (3): {
                record->glowMap = tmp;
                break;
            }
            case (4): {
                record->parallaxMap = tmp;
                break;
            }
            case (5): {
                record->environmentMap = tmp;
                break;
            }
            }
            fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);
        }
    }

    //DODT
    if (strncmp(subrecordHead->Type, "DODT", 4) == 0) {
        constructor              = GET_CONSTRUCTOR(Subrecord, "DODT");
        DODTSubrecord* decalData = (DODTSubrecord*)constructor(esm_file);
        record->decalData        = *decalData;
        free(decalData);
        log_subrecord(subrecordHead);
        fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);
    }

    //DNAM
    if (strncmp(subrecordHead->Type, "DNAM", 4) == 0) {
        uint16_t dnam;
        fread(&dnam, sizeof(uint16_t), 1, esm_file);
        record->flags = dnam;
        log_subrecord(subrecordHead);
        log_debug("Flags: 0x%04x", record->flags);
    } else {
        fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);
    }

    log_record(&hdr);
    return (Record*)record;
}

Record* init_GLOB(FILE* esm_file)
{
    MALLOC_WARN(GLOBRecord, record);
    RecordHeader hdr;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    SubrecordHeader subrecordHead;
    fread(&subrecordHead, sizeof(SubrecordHeader), 1, esm_file);

    record->editorId = init_cstring_subrecord(esm_file, &subrecordHead, "Editor ID");
    fread(&subrecordHead, sizeof(SubrecordHeader), 1, esm_file);

    uint8_t fnam;
    fread(&fnam, sizeof(uint8_t), 1, esm_file);
    record->type = fnam;
    log_subrecord(&subrecordHead);
    log_debug("Type: %c", record->type);

    fread(&subrecordHead, sizeof(SubrecordHeader), 1, esm_file);
    uint32_t value;
    fread(&value, sizeof(uint32_t), 1, esm_file);
    record->value.longValue = value;
    log_subrecord(&subrecordHead);

    switch (fnam) {
    case 's':
        log_debug("Global variable short value: %d", record->value.shortValue);
        break;
    case 'l':
        log_debug("Gloal variable long value: %d", record->value.longValue);
        break;
    case 'f':
        log_debug("Global variable float value %f", record->value.floatValue);
        break;
    default:
        log_fatal("Invalid global variable type: 0x%x", record->type);
        break;
    }

    log_record(&hdr);
    return (Record*)record;
}

Record* init_FACT(FILE* esm_file)
{

    MALLOC_WARN(FACTRecord, record);

    RecordHeader hdr;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    uint32_t         end = ftell(esm_file) + hdr.DataSize;
    SubrecordHeader  subrec;
    SubrecordHeader* subrecordHead = &subrec;
    log_record(&hdr);

    fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);
    record->editorId = init_cstring_subrecord(esm_file, subrecordHead, "Editor ID");

    fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);

    if (strncmp(subrecordHead->Type, "FULL", 4) == 0) {
        record->name = init_cstring_subrecord(esm_file, subrecordHead, "Name");
        fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);
    }
    if (strncmp(subrecordHead->Type, "XNAM", 4) == 0) {
        XNAMSubrecord tmp;

        while (strncmp(subrecordHead->Type, "XNAM", 4) == 0) {
            log_subrecord(subrecordHead);
            fread(&(tmp), sizeof(XNAMSubrecord), 1, esm_file);
            arrput(record->relations, tmp);

            log_debug("Relation:");
            log_debug("Faction: %d", tmp.faction);
            log_debug("Modifier: %d", tmp.modifier);
            log_debug("Group combat reaction: %d", tmp.groupCombatReaction);
            fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);
        }
    }

    fread(&(record->data), subrecordHead->DataSize, 1, esm_file);
    log_subrecord(subrecordHead);
    log_debug("Data:");
    log_debug("Flags 1: 0x%02x", record->data.flags_1);
    log_debug("Flags 2: 0x%02x", record->data.flags_2);
    log_debug("Unused bytes: 0x%02x 0x%02x", record->data.unused[0], record->data.unused[1]);

    if (ftell(esm_file) >= end) {
        return (Record*)record;
    }

    // CNAM, RNAM, INAM, WMI1 are optional
    fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);
    log_debug("Current file pointer location: 0x%06x", ftell(esm_file));

    if (strncmp(subrecordHead->Type, "CNAM", 4) == 0) {
        fread(&(record->unused), sizeof(float), 1, esm_file);
        log_subrecord(subrecordHead);
        log_debug("Unused float value: %f", record->unused);
        fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);
    }
    if (strncmp(subrecordHead->Type, "RNAM", 4) == 0) {
        FACT_RankSubrecords tmp;

        while (strncmp(subrecordHead->Type, "RNAM", 4) == 0) {
            tmp.male   = NULL;
            tmp.female = NULL;
            log_subrecord(subrecordHead);

            fread(&tmp.rankNumber, sizeof(uint32_t), 1, esm_file);
            log_debug("Rank number: %d", tmp.rankNumber);
            log_debug("Current file pointer location: 0x%06x", ftell(esm_file));

            fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);

            if (strncmp(subrecordHead->Type, "MNAM", 4)) {
                // One fucking edge case
                if (strncmp(subrecordHead->Type, "FNAM", 4) == 0) {
                    tmp.female = init_cstring_subrecord(esm_file, subrecordHead, "Female");
                }
                arrput(record->rank, tmp);
                continue;
            }

            tmp.male = init_cstring_subrecord(esm_file, subrecordHead, "Male");
            log_debug("Current file pointer location: 0x%06x", ftell(esm_file));

            fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);

            if (strncmp(subrecordHead->Type, "FNAM", 4)) {
                arrput(record->rank, tmp);
                continue;
            }

            log_debug("Current file pointer location: 0x%06x", ftell(esm_file));
            tmp.female = init_cstring_subrecord(esm_file, subrecordHead, "Female");

            fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);
            if (strncmp(subrecordHead->Type, "INAM", 4) == 0) {
                fread(&tmp.insignia, sizeof(uint32_t), 1, esm_file);
                log_debug("Insignia: %u", tmp.insignia);
                fread(subrecordHead, sizeof(SubrecordHeader), 1, esm_file);
            }

            arrput(record->rank, tmp);
        }
    }
    if (strncmp(subrecordHead->Type, "WMI1", 4) == 0) {
        log_debug("%s", "WMI");
        fread(&(record->reputation), sizeof(formid), 1, esm_file);
    }
    // Rewind to proper position
    if (ftell(esm_file) > end) {
        fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);
    }
    return (Record*)record;
}

Record* init_MICN(FILE* esm_file)
{
    MALLOC_WARN(MICN, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    uint32_t dataStart = ftell(esm_file);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    record->editorID = init_cstring_subrecord(esm_file, &subheader, "EDID");

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    record->largeIconFilename = init_cstring_subrecord(esm_file, &subheader, "ICON");

    if (hdr.DataSize == (ftell(esm_file) - dataStart)) {
        return (Record*)record;
    }

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    record->largeIconFilename = init_cstring_subrecord(esm_file, &subheader, "MICO");

    return (Record*)record;
}

Record* init_CLAS(FILE* esm_file)
{
    MALLOC_WARN(CLASRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    uint32_t dataStart = ftell(esm_file);
    log_record(&hdr);
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    record->editorID = init_cstring_subrecord(esm_file, &subheader, "EDID");

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    record->fullName = init_cstring_subrecord(esm_file, &subheader, "FULL");

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    record->description = init_cstring_subrecord(esm_file, &subheader, "DESC");

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    fread(&(record->data), sizeof(DATASubrecord), 1, esm_file);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    dataStart = ftell(esm_file);
    fread(&(record->attr), sizeof(ATTRSubrecord), 1, esm_file);
    assert((dataStart + subheader.DataSize) == ftell(esm_file));

    return (Record*)record;
}

Record* init_HDPT(FILE* esm_file)
{
    MALLOC_WARN(HDPTRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    log_record(&hdr);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "EDID", 4) == 0);
    record->editorID = init_cstring_subrecord(esm_file, &subheader, "Editor ID");

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FULL", 4) == 0);
    record->name = init_cstring_subrecord(esm_file, &subheader, "Name");

    SubrecordConstructor* func = GET_CONSTRUCTOR(Subrecord, "MODL");
    if (func == NULL) {
        log_fatal("Error while fetching MODL constructor");
        sdsfree(record->editorID);
        sdsfree(record->name);
        free(record);
    }
    record->modelData = (ModelDataSubrecord*)func(esm_file);
    if (record->modelData == NULL) {
        return NULL;
    }

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "DATA", 4) == 0);
    fread(&(record->flag), sizeof(uint8_t), 1, esm_file);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);

    while (strncmp(subheader.Type, "HNAM", 4) == 0) {
        fread(&(record->extraParts), sizeof(formid), 1, esm_file);
        fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    }
    fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);

    return (Record*)record;
}

Record* init_HAIR(FILE* esm_file)
{
    MALLOC_WARN(HAIRRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "EDID", 4) == 0);
    record->editorID = init_cstring_subrecord(esm_file, &subheader, "Editor ID");

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FULL", 4) == 0);
    record->name = init_cstring_subrecord(esm_file, &subheader, "Name");

    SubrecordConstructor* func = GET_CONSTRUCTOR(Subrecord, "MODL");
    if (func == NULL) {
        log_fatal("Error while fetching MODL constructor");
        sdsfree(record->editorID);
        sdsfree(record->name);
        free(record);
    }
    record->modelData = (ModelDataSubrecord*)func(esm_file);
    if (record->modelData == NULL) {
        return NULL;
    }

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "ICON", 4) == 0);
    record->texture = init_cstring_subrecord(esm_file, &subheader, "Texture");

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "DATA", 4) == 0);
    fread(&(record->flag), sizeof(uint8_t), 1, esm_file);

    return (Record*)record;
}

Record* init_EYES(FILE* esm_file)
{
    MALLOC_WARN(EYESRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "EDID", 4) == 0);
    record->editorID = init_cstring_subrecord(esm_file, &subheader, "Editor ID");

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FULL", 4) == 0);
    record->name = init_cstring_subrecord(esm_file, &subheader, "Name");

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    if (strncmp(subheader.Type, "ICON", 4) == 0) {
        record->texture = init_cstring_subrecord(esm_file, &subheader, "Texture");
    } else {
        fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);
    }

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "DATA", 4) == 0);
    fread(&(record->flag), sizeof(uint8_t), 1, esm_file);

    return (Record*)record;
}

Record* init_RACE(FILE* esm_file)
{
    MALLOC_WARN(RACERecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "EDID", 4) == 0);
    record->editorID = init_cstring_subrecord(esm_file, &subheader, "Editor ID");

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FULL", 4) == 0);
    record->name = init_cstring_subrecord(esm_file, &subheader, "Editor ID");

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "DESC", 4) == 0);
    log_subrecord(&subheader);
    if (subheader.DataSize > 1) {
        record->editorID = init_cstring_subrecord(esm_file, &subheader, "Editor ID");
    } else {
        fseek(esm_file, 1, SEEK_CUR);
    }

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    if (strncmp(subheader.Type, "XNAM", 4) == 0) {
        XNAMSubrecord tmp;

        while (strncmp(subheader.Type, "XNAM", 4) == 0) {
            log_subrecord(&subheader);
            fread(&(tmp), sizeof(XNAMSubrecord), 1, esm_file);
            arrput(record->relations, tmp);

            log_debug("Relation:");
            log_debug("Faction: %d", tmp.faction);
            log_debug("Modifier: %d", tmp.modifier);
            log_debug("Group combat reaction: %d", tmp.groupCombatReaction);
            fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
        }
    }

    assert(strncmp(subheader.Type, "DATA", 4) == 0);
    log_subrecord(&subheader);
    fread(&(record->raceData), sizeof(RaceDataSubrecord), 1, esm_file);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    if (strncmp(subheader.Type, "ONAM", 4) == 0) {
        fread(&(record->older), sizeof(formid), 1, esm_file);
        log_subrecord(&subheader);

        fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    }
    if (strncmp(subheader.Type, "YNAM", 4) == 0) {
        fread(&(record->younger), sizeof(formid), 1, esm_file);
        log_subrecord(&subheader);

        fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    }

    // NAM2 marker, should contain no data
    assert(strncmp(subheader.Type, "NAM2", 4) == 0);
    assert(subheader.DataSize == 0);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "VTCK", 4) == 0);
    fread(&(record->voices), sizeof(RaceVoices), 1, esm_file);
    log_subrecord(&subheader);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);

    // Hair style and color is optional, e.g. ghouls
    if (strncmp(subheader.Type, "DNAM", 4) == 0) {
        fread(&(record->defaultHair), sizeof(RaceDefaultHairStyle), 1, esm_file);
        log_subrecord(&subheader);
        fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    }

    if (strncmp(subheader.Type, "CNAM", 4) == 0) {
        fread(&(record->defaultHairColor), sizeof(RaceDefaultHairColor), 1, esm_file);
        log_subrecord(&subheader);
        fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    }

    assert(strncmp(subheader.Type, "PNAM", 4) == 0);
    fread(&(record->faceGenMainClamp), sizeof(float), 1, esm_file);
    log_subrecord(&subheader);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "UNAM", 4) == 0);
    fread(&(record->faceGenFaceClamp), sizeof(float), 1, esm_file);
    log_subrecord(&subheader);

    // Dummy ATTR read, functionality still unknown
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "ATTR", 4) == 0);
    fseek(esm_file, subheader.DataSize, SEEK_CUR);
    log_subrecord(&subheader);

    // Head data marker, should contain no data
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "NAM0", 4) == 0);
    assert(subheader.DataSize == 0);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "MNAM", 4) == 0);
    log_subrecord(&subheader);
    record->maleHeadParts = init_ModelPartCollection(esm_file);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FNAM", 4) == 0);
    log_subrecord(&subheader);
    record->femaleHeadParts = init_ModelPartCollection(esm_file);

    // Body data marker, should contain no data
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "NAM1", 4) == 0);
    assert(subheader.DataSize == 0);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "MNAM", 4) == 0);
    log_subrecord(&subheader);
    record->maleBodyParts = init_ModelPartCollection(esm_file);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FNAM", 4) == 0);
    log_subrecord(&subheader);
    record->femaleBodyParts = init_ModelPartCollection(esm_file);

    // Hair formids
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "HNAM", 4) == 0);
    log_subrecord(&subheader);
    MALLOC_N_WARN(formid, subheader.DataSize, hair);
    fread(hair, subheader.DataSize, 1, esm_file);
    record->hair = hair;

    // Eyes formids
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "ENAM", 4) == 0);
    log_subrecord(&subheader);
    MALLOC_N_WARN(formid, subheader.DataSize, eyes);
    fread(eyes, subheader.DataSize, 1, esm_file);
    record->eyes = eyes;

    // FaceGen male data marker, should contain no data
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "MNAM", 4) == 0);
    assert(subheader.DataSize == 0);
    log_subrecord(&subheader);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FGGS", 4) == 0);
    log_subrecord(&subheader);
    MALLOC_N_WARN(uint8_t, subheader.DataSize, maleSym);
    fread(maleSym, subheader.DataSize, 1, esm_file);
    record->maleFaceGenGeomSymm = maleSym;

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FGGA", 4) == 0);
    log_subrecord(&subheader);
    MALLOC_N_WARN(uint8_t, subheader.DataSize, maleAsym);
    fread(maleAsym, subheader.DataSize, 1, esm_file);
    record->maleFaceGenGeomAsymm = maleAsym;

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FGTS", 4) == 0);
    log_subrecord(&subheader);
    MALLOC_N_WARN(uint8_t, subheader.DataSize, maleTexSym);
    fread(maleTexSym, subheader.DataSize, 1, esm_file);
    record->maleFaceGenTexSymm = maleTexSym;

    // Dummy SNAM read
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "SNAM", 4) == 0);
    log_subrecord(&subheader);
    fseek(esm_file, subheader.DataSize, SEEK_CUR);

    // FaceGen female data marker, should contain no data
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FNAM", 4) == 0);
    assert(subheader.DataSize == 0);
    log_subrecord(&subheader);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FGGS", 4) == 0);
    log_subrecord(&subheader);
    MALLOC_N_WARN(uint8_t, subheader.DataSize, femaleSym);
    fread(femaleSym, subheader.DataSize, 1, esm_file);
    record->femaleFaceGenGeomSymm = femaleSym;

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FGGA", 4) == 0);
    log_subrecord(&subheader);
    MALLOC_N_WARN(uint8_t, subheader.DataSize, femaleAsym);
    fread(femaleAsym, subheader.DataSize, 1, esm_file);
    record->femaleFaceGenGeomAsymm = femaleAsym;

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FGTS", 4) == 0);
    log_subrecord(&subheader);
    MALLOC_N_WARN(uint8_t, subheader.DataSize, femaleTexSym);
    fread(femaleTexSym, subheader.DataSize, 1, esm_file);
    record->femaleFaceGenTexSymm = femaleTexSym;

    // Dummy SNAM read
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "SNAM", 4) == 0);
    log_subrecord(&subheader);
    fseek(esm_file, subheader.DataSize, SEEK_CUR);

    return (Record*)record;
}

Record* init_SOUN(FILE* esm_file) {
  MALLOC_WARN(SOUNRecord, record);
  RecordHeader hdr;
  SubrecordHeader subheader;

  fread(&hdr, sizeof(RecordHeader), 1, esm_file);
  FILL_BASE_RECORD_INFO(hdr, record);

  int readCounter = 0;
  uint32_t  end = ftell(esm_file) + hdr.DataSize;

  log_record(&hdr);

  fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
  assert(strncmp(subheader.Type, "EDID", 4) == 0);
  record->editorID = init_cstring_subrecord(esm_file, &subheader, "Editor ID");


  fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);

  if(strncmp(subheader.Type, "OBND", 4) == 0) {
    log_subrecord(&subheader);
    readCounter =
      fread(&(record->objectBounds), sizeof(OBNDSubrecord), 1, esm_file);
    assert(readCounter == 1);
    log_OBND(&(record->objectBounds));
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
  } else {
    record->objectBounds.x1 = 0;
    record->objectBounds.y1 = 0;
    record->objectBounds.z1 = 0;
    record->objectBounds.x2 = 0;
    record->objectBounds.y2 = 0;
    record->objectBounds.z2 = 0;    
  }

  if(strncmp(subheader.Type, "FNAM", 4) == 0) {
    log_subrecord(&subheader);
    record->soundFilename =
        init_cstring_subrecord(esm_file, &subheader, "Sound filename");
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
  } else {
    record->soundFilename = NULL;
  }


  if(strncmp(subheader.Type, "RNAM", 4) == 0) {
    log_subrecord(&subheader);
    readCounter =
        fread(&(record->randomChangePercentage), sizeof(uint8_t), 1, esm_file);
    assert(readCounter == 1);
    log_debug("Random change percentage: %d", record->randomChangePercentage);
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
  } else {
    record->randomChangePercentage = 0;
  }


  if(strncmp(subheader.Type, "SNDD", 4) == 0) {
    log_subrecord(&subheader);
    readCounter = fread(&(record->soundData), sizeof(SoundData), 1, esm_file);
    assert(readCounter == 1);
    log_SNDD(&(record->soundData));

  } else {
    assert(strncmp(subheader.Type, "SNDX", 4) == 0);
    log_subrecord(&subheader);
    readCounter = fread(&(record->soundData), sizeof(SoundEx), 1, esm_file);
    assert(readCounter == 1);

    record->soundData.attenuationPoints[0] = 0;
    record->soundData.attenuationPoints[1] = 0;
    record->soundData.attenuationPoints[2] = 0;
    record->soundData.attenuationPoints[3] = 0;
    record->soundData.attenuationPoints[4] = 0;
    record->soundData.reverbAttenuationControl = 0;
    record->soundData.priority = 0;
    record->soundData.x = 0;
    record->soundData.y = 0;

    if(ftell(esm_file) >= end) {
        log_SNDD(&(record->soundData));
        return (Record*) record;
    }

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    if(strncmp(subheader.Type, "ANAM", 4) == 0) {
        log_subrecord(&subheader);
        readCounter = fread(&(record->soundData.attenuationPoints), sizeof(int16_t), 5, esm_file);
        assert(readCounter == 5);
        if(ftell(esm_file) >= end) {
            log_SNDD(&(record->soundData));
            return (Record*) record;
        }
        fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    }

    if(strncmp(subheader.Type, "GNAM", 4) == 0) {
        log_subrecord(&subheader);
        readCounter = fread(&(record->soundData.reverbAttenuationControl), sizeof(int16_t), 1, esm_file);
        assert(readCounter == 1);
        if(ftell(esm_file) >= end) {
            log_SNDD(&(record->soundData));
            return (Record*) record;
        }
    }

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    log_subrecord(&subheader);
    assert(strncmp(subheader.Type, "HNAM", 4) == 0);
    readCounter = fread(&(record->soundData.priority), sizeof(int32_t), 1, esm_file);
    assert(readCounter == 1);

    log_SNDD(&(record->soundData));
  }

  return (Record*)record;
}


#define ASCP_NAM_SUBHEADER(subrecordName, value, type, subheader, esm_file, \
                           logging_format)                                  \
  fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);                  \
  log_subrecord(&subheader);                                                \
  assert(strncmp(subheader.Type, subrecordName, 4) == 0);                   \
  readCounter = fread(&(value), sizeof(type), 1, esm_file);                \
  assert(readCounter == 1);                                                 \
  log_debug(logging_format, value);

Record* init_ASPC(FILE* esm_file) {
  MALLOC_WARN(ASPCRecord, record);
  RecordHeader hdr;
  SubrecordHeader subheader;

  fread(&hdr, sizeof(RecordHeader), 1, esm_file);
  FILL_BASE_RECORD_INFO(hdr, record);

  int readCounter = 0;

  log_record(&hdr);

  fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
  assert(strncmp(subheader.Type, "EDID", 4) == 0);
  record->editorID = init_cstring_subrecord(esm_file, &subheader, "Editor ID");

  fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);

  assert(strncmp(subheader.Type, "OBND", 4) == 0);
  log_subrecord(&subheader);
  readCounter =
      fread(&(record->objectBounds), sizeof(OBNDSubrecord), 1, esm_file);
  assert(readCounter == 1);
  log_OBND(&(record->objectBounds));

  ASCP_NAM_SUBHEADER("SNAM", record->dawn_default, formid, subheader, esm_file, "Dawn: %d");
  ASCP_NAM_SUBHEADER("SNAM", record->afternoon, formid, subheader, esm_file, "Afternoon: %d");
  ASCP_NAM_SUBHEADER("SNAM", record->dusk, formid, subheader, esm_file, "Dusk: %d");
  ASCP_NAM_SUBHEADER("SNAM", record->night, formid, subheader, esm_file, "Night: %d");
  ASCP_NAM_SUBHEADER("SNAM", record->walla, formid, subheader, esm_file, "Walla: %d");
  ASCP_NAM_SUBHEADER("WNAM", record->wallaTriggerCount, uint32_t, subheader, esm_file, "Walla trigger count: %d");

  fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);                 
  if (strncmp(subheader.Type, "RDAT", 4) == 0) {                    
    readCounter = fread(&(record->regionSound), sizeof(formid), 1, esm_file);              \
    assert(readCounter == 1);                                              
  } else {                                                                 
    fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);                              
    record->regionSound = 0;                                                             \
  }                                                                        
  log_debug("Use sound from region: %d", record->regionSound);

  ASCP_NAM_SUBHEADER("ANAM", record->environmentType, uint32_t, subheader, esm_file, "Environment type: %d");
  ASCP_NAM_SUBHEADER("INAM", record->isInterior, uint32_t, subheader, esm_file, "Is interior: %d");

  return (Record*)record;
}

#define MGEF_OPTIONAL_CSTRING_RECORD(subrecordName, value, subheader,   \
                                     esm_file, logging_name)            \
  fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);              \
  if (strncmp(subheader.Type, subrecordName, 4) == 0) {                 \
    value = init_cstring_subrecord(esm_file, &subheader, logging_name); \
  } else {                                                              \
    value = NULL;                                                       \
    fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);                \
  }

#define MGEF_CSTRING_RECORD(subrecordName, value, subheader, esm_file, \
                            logging_name)                            \
  fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);             \
  assert(strncmp(subheader.Type, subrecordName, 4) == 0);              \
  value = init_cstring_subrecord(esm_file, &subheader, logging_name);

Record* init_MGEF(FILE* esm_file) {
  MALLOC_WARN(MGEFRecord, record);
  RecordHeader hdr;
  SubrecordHeader subheader;

  fread(&hdr, sizeof(RecordHeader), 1, esm_file);
  FILL_BASE_RECORD_INFO(hdr, record);
  log_record(&hdr);
  int readCounter = 0;

  MGEF_CSTRING_RECORD("EDID", record->editorID, subheader, esm_file,
                      "Editor ID");
  MGEF_OPTIONAL_CSTRING_RECORD("FULL", record->name, subheader, esm_file, "Name");
  MGEF_CSTRING_RECORD("DESC", record->description, subheader, esm_file,
                      "Description");
  MGEF_OPTIONAL_CSTRING_RECORD("ICON", record->largeIconFilename, subheader, esm_file,
                      "Large icon filename");
  MGEF_OPTIONAL_CSTRING_RECORD("MICO", record->smallIconFilename, subheader, esm_file,
                      "Small icon filename");

  SubrecordConstructor* func = GET_CONSTRUCTOR(Subrecord, "MODL");
  if (func == NULL) {
    log_fatal("Error while fetching MODL constructor");
    sdsfree(record->editorID);
    sdsfree(record->name);
    free(record);
  }
  record->modelData = (ModelDataSubrecord*)func(esm_file);
  if (record->modelData == NULL) {
    return NULL;
  }

  fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
  assert(strncmp(subheader.Type, "DATA", 4) == 0);
  readCounter =
      fread(&(record->magicEffectData), sizeof(MagicEffectData), 1, esm_file);
  assert(readCounter == 1);
  log_MagicEffectData(&(record->magicEffectData));

  return (Record*)record;
}

void free_MGEF(Record* record) {
    MGEFRecord* magicEffect = (MGEFRecord*)record;
    sdsfree(magicEffect->editorID);
    sdsfree(magicEffect->name);
    sdsfree(magicEffect->description);
    sdsfree(magicEffect->largeIconFilename);
    sdsfree(magicEffect->smallIconFilename);
    SubrecordDestructor* func = GET_DESTRUCTOR(Subrecord, "MODL");
    func((Subrecord*)magicEffect->modelData);
    free(magicEffect);
}

void free_ASPC(Record* record) {
    ASPCRecord* acousticSpace = (ASPCRecord*)record;
    sdsfree(acousticSpace->editorID);
}


void free_SOUN(Record* record) {
    SOUNRecord* sound = (SOUNRecord*)record;

    sdsfree(sound->editorID);
    if(sound->soundFilename != NULL) {
        sdsfree(sound->soundFilename);
    }

    free(sound);
}

void free_RACE(Record* record)
{
    RACERecord* race = (RACERecord*)record;

    if (race->description != NULL) {
        sdsfree(race->description);
    }

    if (race->relations != NULL) {
        arrfree(race->relations);
    }

    free_ModelPartCollection(race->maleBodyParts);
    free_ModelPartCollection(race->maleHeadParts);
    free_ModelPartCollection(race->femaleBodyParts);
    free_ModelPartCollection(race->femaleHeadParts);

    if (race->hair != NULL) {
        free(race->hair);
    }

    if (race->eyes != NULL) {
        free(race->eyes);
    }

    free(race->maleFaceGenGeomSymm);
    free(race->maleFaceGenGeomAsymm);
    free(race->maleFaceGenTexSymm);
    free(race->femaleFaceGenGeomSymm);
    free(race->femaleFaceGenGeomAsymm);
    free(race->femaleFaceGenTexSymm);

    sdsfree(race->editorID);
    sdsfree(race->name);

    free(race);
}

void free_EYES(Record* record)
{
    EYESRecord* eyes = (EYESRecord*)record;

    sdsfree(eyes->editorID);
    sdsfree(eyes->name);

    if (eyes->texture != NULL) {
        sdsfree(eyes->texture);
    }

    free(eyes);
}

void free_FACT(Record* record)
{
    FACTRecord* fact = (FACTRecord*)record;

    arrfree(fact->relations);

    uint32_t len = arrlenu(fact->rank);
    for (uint32_t i = 0; i < len; i++) {
        if (fact->rank[i].male != NULL) {
            sdsfree(fact->rank[i].male);
        }
        if (fact->rank[i].female != NULL) {
            sdsfree(fact->rank[i].female);
        }
    }
    arrfree(fact->rank);

    sdsfree(fact->editorId);
    if (fact->name)
        sdsfree(fact->name);

    free(fact);
}

void free_HAIR(Record* record)
{
    HAIRRecord* hair = (HAIRRecord*)record;

    sdsfree(hair->editorID);
    sdsfree(hair->name);
    sdsfree(hair->texture);

    SubrecordDestructor* func = GET_DESTRUCTOR(Subrecord, "MODL");
    func((Subrecord*)hair->modelData);

    free(hair);
}

void free_HDPT(Record* record)
{
    HDPTRecord* hdpt = (HDPTRecord*)record;

    sdsfree(hdpt->editorID);
    sdsfree(hdpt->name);

    SubrecordDestructor* func = GET_DESTRUCTOR(Subrecord, "MODL");
    func((Subrecord*)hdpt->modelData);

    free(hdpt);
}

void free_TES4(Record* record)
{
    TES4Record*          tmp  = (TES4Record*)record;
    SubrecordDestructor* func = GET_DESTRUCTOR(Subrecord, "HEDR");
    func((Subrecord*)tmp->hedr);

    func = GET_DESTRUCTOR(Subrecord, "CNAM");
    func((Subrecord*)tmp->cnam);

    free(record);
}

void free_GMST(Record* record)
{
    GMSTRecord* gmst_record = (GMSTRecord*)record;
    if ((gmst_record->editorId)[0] == 's') {
        sdsfree(gmst_record->value.stringValue);
    }
    sdsfree(gmst_record->editorId);
    free(record);
}

void free_MICN(Record* record)
{
    MICN* rec = (MICN*)record;

    sdsfree(rec->largeIconFilename);
    sdsfree(rec->editorID);

    if (rec->smallIconFilename) {
        sdsfree(rec->smallIconFilename);
    }

    free(rec);
}

void free_TXST(Record* record)
{
    TXSTRecord* txst = (TXSTRecord*)record;
    sdsfree(txst->editorId);

    if (txst->baseImage_transparency != NULL) {
        sdsfree(txst->baseImage_transparency);
    }
    if (txst->normalMap_specular != NULL) {
        sdsfree(txst->normalMap_specular);
    }
    if (txst->environmentMapMask != NULL) {
        sdsfree(txst->environmentMapMask);
    }
    if (txst->glowMap != NULL) {
        sdsfree(txst->glowMap);
    }
    if (txst->parallaxMap != NULL) {
        sdsfree(txst->parallaxMap);
    }
    if (txst->environmentMap != NULL) {
        sdsfree(txst->environmentMap);
    }

    free(txst);
}

void free_GLOB(Record* record)
{
    GLOBRecord* glob_record = (GLOBRecord*)record;
    sdsfree(glob_record->editorId);
    free(glob_record);
}

void free_CLAS(Record* record)
{
    CLASRecord* clas = (CLASRecord*)record;

    sdsfree(clas->editorID);
    sdsfree(clas->description);
    sdsfree(clas->fullName);

    free(clas);
}

void Record_init_constructor_map()
{
    ADD_CONSTRUCTOR(Record, "TES4", init_TES4);
    ADD_CONSTRUCTOR(Record, "GMST", init_GMST);
    ADD_CONSTRUCTOR(Record, "TXST", init_TXST);
    ADD_CONSTRUCTOR(Record, "GLOB", init_GLOB);
    ADD_CONSTRUCTOR(Record, "MICN", init_MICN);
    ADD_CONSTRUCTOR(Record, "CLAS", init_CLAS);
    ADD_CONSTRUCTOR(Record, "FACT", init_FACT);
    ADD_CONSTRUCTOR(Record, "HDPT", init_HDPT);
    ADD_CONSTRUCTOR(Record, "HAIR", init_HAIR);
    ADD_CONSTRUCTOR(Record, "EYES", init_EYES);
    ADD_CONSTRUCTOR(Record, "RACE", init_RACE);
    ADD_CONSTRUCTOR(Record, "SOUN", init_SOUN);
    ADD_CONSTRUCTOR(Record, "ASPC", init_ASPC);
    ADD_CONSTRUCTOR(Record, "MGEF", init_MGEF);
}

void Record_init_destructor_map()
{
    ADD_DESTRUCTOR(Record, "TES4", free_TES4);
    ADD_DESTRUCTOR(Record, "GMST", free_GMST);
    ADD_DESTRUCTOR(Record, "TXST", free_TXST);
    ADD_DESTRUCTOR(Record, "GLOB", free_GLOB);
    ADD_DESTRUCTOR(Record, "MICN", free_MICN);
    ADD_DESTRUCTOR(Record, "CLAS", free_CLAS);
    ADD_DESTRUCTOR(Record, "FACT", free_FACT);
    ADD_DESTRUCTOR(Record, "HDPT", free_HDPT);
    ADD_DESTRUCTOR(Record, "HAIR", free_HAIR);
    ADD_DESTRUCTOR(Record, "EYES", free_EYES);
    ADD_DESTRUCTOR(Record, "RACE", free_RACE);
    ADD_DESTRUCTOR(Record, "SOUN", free_SOUN);
    ADD_DESTRUCTOR(Record, "ASPC", free_ASPC);
    ADD_DESTRUCTOR(Record, "MGEF", free_MGEF);
}

Record* recordnew(FILE* f, sds type)
{
    Record*            ret  = NULL;
    RecordConstructor* func = GET_CONSTRUCTOR(Record, type);

    if (func == NULL) {
        log_warn("Record type %s not yet implemented.", type);
        return NULL;
    }

    ret = func(f);
    return ret;
}
void recordfree(Record* record)
{
    sds               type = sdsnewlen(record->Type, 4);
    RecordDestructor* func = GET_DESTRUCTOR(Record, type);
    func(record);
    sdsfree(type);
}
