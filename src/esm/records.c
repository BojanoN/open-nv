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

void free_HAIR(Record* record)
{
    HAIRRecord* hair = (HAIRRecord*)record;

    sdsfree(hair->editorID);
    sdsfree(hair->name);
    sdsfree(hair->texture);

    SubrecordDestructor* func = GET_DESTRUCTOR(Subrecord, "MODL");
    func((Subrecord*)hair->modelData);

    free(hair->modelData);
    free(hair);
}

void free_HDPT(Record* record)
{
    HDPTRecord* hdpt = (HDPTRecord*)record;

    sdsfree(hdpt->editorID);
    sdsfree(hdpt->name);

    SubrecordDestructor* func = GET_DESTRUCTOR(Subrecord, "MODL");
    func((Subrecord*)hdpt->modelData);

    free(hdpt->modelData);
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
