#include "records.h"
#include "util/memutils.h"

DECLARE_FUNCTION_MAPS(Record);

FETCH_CONSTRUCTOR_MAP(Subrecord);
FETCH_DESTRUCTOR_MAP(Subrecord);

Record* init_TES4(FILE* esm_file)
{
    MALLOC_WARN(TES4Record, ret);

    fread(&(ret->base), RECORD_SIZE, 1, esm_file);

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

    log_record(&(ret->base));

    return (Record*)ret;
}

sds init_cstring_subrecord(FILE* esm_file, Subrecord* subrecordHead, const char* loggingName)
{
    char* cstring = malloc(subrecordHead->DataSize);
    fread(cstring, subrecordHead->DataSize, 1, esm_file);
    sds subrecord = sdsnewlen(cstring, subrecordHead->DataSize);
    free(cstring);
    log_subrecord_new(subrecordHead);
    log_debug("%s: %s", loggingName, subrecord);
    return subrecord;
}

Record* init_GMST(FILE* esm_file)
{
    MALLOC_WARN(GMSTRecord, record);

    fread(&(record->base), RECORD_SIZE, 1, esm_file);
    //EDID
    Subrecord  subrec;
    Subrecord* subrecordHead = &subrec;
    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);

    record->editorId = init_cstring_subrecord(esm_file, subrecordHead, "Editor ID");

    //DATA
    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);

    size_t dataSize = subrecordHead->DataSize;

    log_subrecord_new(subrecordHead);

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

    log_record(&(record->base));

    return (Record*)record;
}

Record* init_TXST(FILE* esm_file)
{
    MALLOC_WARN(TXSTRecord, record);
    fread(&(record->base), RECORD_SIZE, 1, esm_file);

    //EDID
    Subrecord  subrecord;
    Subrecord* subrecordHead = &subrecord;
    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);

    record->editorId = init_cstring_subrecord(esm_file, subrecordHead, "Editor ID");
    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    sds type = sdsnewlen(subrecordHead->Type, 4);

    SubrecordConstructor* constructor = NULL;
    //OBND
    if (strcmp(type, "OBND") == 0) {
        constructor                 = GET_CONSTRUCTOR(Subrecord, "OBND");
        OBNDSubrecord* objectBounds = (OBNDSubrecord*)constructor(esm_file);
        record->objectBounds        = *objectBounds;
        free(objectBounds);
        log_subrecord_new(subrecordHead);
        fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
        sdsfree(type);
        type = sdsnewlen(subrecordHead->Type, 4);
    }

    //TX00-TX05
    const char* optionalCstringSubrecordTypes[] = { "TX00", "TX01", "TX02",
        "TX03", "TX04", "TX05" };

    sds optionalCstringSubrecordDestinations[] = {
        record->baseImage_transparency,
        record->normalMap_specular,
        record->environmentMapMask,
        record->glowMap,
        record->parallaxMap,
        record->environmentMap
    };

    const char* optionalNames[] = {
        "Base image/Transparency",
        "Normal map/Specular",
        "Environment map mask",
        "Glow map",
        "Parallax map",
        "Environment map"
    };
    /*
    for (int i = 0; i < 6; i++) {
        if (strcmp(type, optionalCstringSubrecordTypes[i]) == 0) {
            optionalCstringSubrecordDestinations[i] = init_cstring_subrecord(esm_file, subrecordHead, optionalNames[i]);
            fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
            sdsfree(type);
            type = sdsnewlen(subrecordHead->Type, 4);
        } else {
            optionalCstringSubrecordDestinations[i] = NULL;
        }
    }*/
    for (int i = 0; i < 6; i++) {
        if (strcmp(type, optionalCstringSubrecordTypes[i]) == 0) {
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
            fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
            sdsfree(type);
            type = sdsnewlen(subrecordHead->Type, 4);
        }
    }

    //DODT
    if (strcmp(type, "DODT") == 0) {
        constructor              = GET_CONSTRUCTOR(Subrecord, "DODT");
        DODTSubrecord* decalData = (DODTSubrecord*)constructor(esm_file);
        record->decalData        = *decalData;
        free(decalData);
        log_subrecord_new(subrecordHead);
        fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
        sdsfree(type);
        type = sdsnewlen(subrecordHead->Type, 4);
    }

    //DNAM
    if (strcmp(type, "DNAM") == 0) {
        uint16_t dnam;
        fread(&dnam, sizeof(uint16_t), 1, esm_file);
        record->flags = dnam;
        log_subrecord_new(subrecordHead);
        log_debug("Flags: 0x%04x", record->flags);
    } else {
        fseek(esm_file, -sizeof(Subrecord), SEEK_CUR);
    }

    log_record(&(record->base));
    sdsfree(type);
    return (Record*)record;
}

Record* init_GLOB(FILE* esm_file)
{
    MALLOC_WARN(GLOBRecord, record);

    fread(&(record->base), RECORD_SIZE, 1, esm_file);

    Subrecord subrecordHead;
    fread(&subrecordHead, sizeof(Subrecord), 1, esm_file);

    record->editorId = init_cstring_subrecord(esm_file, &subrecordHead, "Editor ID");
    fread(&subrecordHead, sizeof(Subrecord), 1, esm_file);

    uint8_t fnam;
    fread(&fnam, sizeof(uint8_t), 1, esm_file);
    record->type = fnam;
    log_subrecord_new(&subrecordHead);
    log_debug("Type: %c", record->type);

    fread(&subrecordHead, sizeof(Subrecord), 1, esm_file);
    uint32_t value;
    fread(&value, sizeof(uint32_t), 1, esm_file);
    record->value.longValue = value;
    log_subrecord_new(&subrecordHead);

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

    log_record(&(record->base));
    return (Record*)record;
}

Record* init_FACT(FILE* esm_file)
{

    MALLOC_WARN(FACTRecord, record);

    fread(&(record->base), RECORD_SIZE, 1, esm_file);
    MALLOC_WARN(Subrecord, subrecordHead);

    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    record->editorId = init_cstring_subrecord(esm_file, subrecordHead, "Editor ID");

    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    record->name = init_cstring_subrecord(esm_file, subrecordHead, "Name");

    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    sds type = sdsnewlen(subrecordHead->Type, 4);

    XNAMSubrecord* xnams = NULL;
    if (strcmp(type, "XNAM") == 0) {
        int i = 0;
        while (strcmp(type, "XNAM")) {
            log_subrecord_new(subrecordHead);

            xnams = realloc(xnams, (i + 1) * sizeof(XNAMSubrecord));
            fread(&(xnams[i]), sizeof(XNAMSubrecord), 1, esm_file);
            log_debug("Relation:");
            log_debug("Faction: %d", (&(xnams[i]))->faction);
            log_debug("Modifier: %d", (&(xnams[i]))->modifier);
            log_debug("Group combat reaction: %d", (&(xnams[i]))->groupCombatReaction);
            fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
            i++;
        }
        record->relations       = xnams;
        record->relationsLength = i + 1;
    }

    fread(&(record->data), sizeof(FACT_DATASubrecord), 1, esm_file);
    log_subrecord_new(subrecordHead);
    log_debug("Data:");
    log_debug("Flags 1: 0x%02x", record->data.flags_1);
    log_debug("Flags 2: 0x%02x", record->data.flags_2);
    log_debug("Unused bytes: 0x%02x 0x%02x", record->data.unused[0], record->data.unused[1]);

    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    type = sdsnewlen(subrecordHead->Type, 4);
    if (strcmp(type, "CNAM") == 0) {
        fread(&(record->unused), sizeof(float), 1, esm_file);
        log_subrecord_new(subrecordHead);
        log_debug("Unused float value: %f", record->unused);
    }

    FACT_RankSubrecords* ranks = NULL;

    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    type = sdsnewlen(subrecordHead->Type, 4);
    if (strcmp(type, "RNAM") == 0) {
        int i = 0;
        while (strcmp(type, "RNAM") == 0) {

            log_subrecord_new(subrecordHead);
            ranks = realloc(ranks, (i + 1) * sizeof(FACT_RankSubrecords));
            fread(&(ranks[i].rankNumber), sizeof(uint32_t), 1, esm_file);
            log_debug("Rank number: %d", ranks->rankNumber);

            fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
            (&(ranks[i]))->male = init_cstring_subrecord(esm_file, subrecordHead, "Male");

            fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
            (&(ranks[i]))->female = init_cstring_subrecord(esm_file, subrecordHead, "Female");

            fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
            if (strcmp(type, "INAM") == 0) {
                (&(ranks[i]))->insignia = init_cstring_subrecord(esm_file, subrecordHead, "Insignia");
                fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
            }
        }
    } else {
        fseek(esm_file, -sizeof(Subrecord), SEEK_CUR);
    }
}

void free_FACT(Record* record)
{
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

Record* init_MICN(FILE* esm_file)
{
    MALLOC_WARN(MICN, record);
    Subrecord subheader;

    fread(&(record->base), sizeof(Record), 1, esm_file);

    uint32_t dataStart = ftell(esm_file);

    fread(&subheader, sizeof(Subrecord), 1, esm_file);
    record->editorID = init_cstring_subrecord(esm_file, &subheader, "EDID");

    fread(&subheader, sizeof(Subrecord), 1, esm_file);
    record->largeIconFilename = init_cstring_subrecord(esm_file, &subheader, "ICON");

    if (record->base.DataSize == (ftell(esm_file) - dataStart)) {
        return (Record*)record;
    }

    fread(&subheader, sizeof(Subrecord), 1, esm_file);
    record->largeIconFilename = init_cstring_subrecord(esm_file, &subheader, "MICO");

    return (Record*)record;
}

Record* init_CLAS(FILE* esm_file)
{
    MALLOC_WARN(CLASRecord, record);
    Subrecord subheader;

    fread(&(record->base), sizeof(Record), 1, esm_file);
    uint32_t dataStart = ftell(esm_file);
    log_record(&(record->base));
    fread(&subheader, sizeof(Subrecord), 1, esm_file);
    record->editorID = init_cstring_subrecord(esm_file, &subheader, "EDID");

    fread(&subheader, sizeof(Subrecord), 1, esm_file);
    record->fullName = init_cstring_subrecord(esm_file, &subheader, "FULL");

    fread(&subheader, sizeof(Subrecord), 1, esm_file);
    record->description = init_cstring_subrecord(esm_file, &subheader, "DESC");

    fread(&subheader, sizeof(Subrecord), 1, esm_file);
    fread(&(record->data), sizeof(DATASubrecord), 1, esm_file);

    fread(&subheader, sizeof(Subrecord), 1, esm_file);
    dataStart = ftell(esm_file);
    fread(&(record->attr), sizeof(ATTRSubrecord), 1, esm_file);
    assert((dataStart + subheader.DataSize) == ftell(esm_file));

    return (Record*)record;
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
}

void Record_init_destructor_map()
{
    ADD_DESTRUCTOR(Record, "TES4", free_TES4);
    ADD_DESTRUCTOR(Record, "GMST", free_GMST);
    ADD_DESTRUCTOR(Record, "TXST", free_TXST);
    ADD_DESTRUCTOR(Record, "GLOB", free_GLOB);
    ADD_DESTRUCTOR(Record, "MICN", free_MICN);
    ADD_DESTRUCTOR(Record, "CLAS", free_CLAS);
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
