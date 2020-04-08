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

    SubrecordConstructor* constructor = NULL;
    //OBND
    if (strncmp(subrecordHead->Type, "OBND", 4) == 0) {
        constructor                 = GET_CONSTRUCTOR(Subrecord, "OBND");
        OBNDSubrecord* objectBounds = (OBNDSubrecord*)constructor(esm_file);
        record->objectBounds        = *objectBounds;
        free(objectBounds);
        log_subrecord_new(subrecordHead);
        fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    }

    //TX00-TX05
    const char* optionalCstringSubrecordTypes[] = { "TX00", "TX01", "TX02",
        "TX03", "TX04", "TX05" };

    const char* optionalNames[] = {
        "Base image/Transparency",
        "Normal map/Specular",
        "Environment map mask",
        "Glow map",
        "Parallax map",
        "Environment map"
    };
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
            fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
        }
    }

    //DODT
    if (strncmp(subrecordHead->Type, "DODT", 4) == 0) {
        constructor              = GET_CONSTRUCTOR(Subrecord, "DODT");
        DODTSubrecord* decalData = (DODTSubrecord*)constructor(esm_file);
        record->decalData        = *decalData;
        free(decalData);
        log_subrecord_new(subrecordHead);
        fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    }

    //DNAM
    if (strncmp(subrecordHead->Type, "DNAM", 4) == 0) {
        uint16_t dnam;
        fread(&dnam, sizeof(uint16_t), 1, esm_file);
        record->flags = dnam;
        log_subrecord_new(subrecordHead);
        log_debug("Flags: 0x%04x", record->flags);
    } else {
        fseek(esm_file, -sizeof(Subrecord), SEEK_CUR);
    }

    log_record(&(record->base));
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
    uint32_t   end = ftell(esm_file) + record->base.DataSize;
    Subrecord  subrec;
    Subrecord* subrecordHead = &subrec;
    log_record(&(record->base));

    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    record->editorId = init_cstring_subrecord(esm_file, subrecordHead, "Editor ID");

    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);

    if (strncmp(subrecordHead->Type, "FULL", 4) == 0) {
        record->name = init_cstring_subrecord(esm_file, subrecordHead, "Name");
        fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    }
    if (strncmp(subrecordHead->Type, "XNAM", 4) == 0) {
        XNAMSubrecord tmp;

        while (strncmp(subrecordHead->Type, "XNAM", 4) == 0) {
            log_subrecord_new(subrecordHead);
            fread(&(tmp), sizeof(XNAMSubrecord), 1, esm_file);
            arrput(record->relations, tmp);

            log_debug("Relation:");
            log_debug("Faction: %d", tmp.faction);
            log_debug("Modifier: %d", tmp.modifier);
            log_debug("Group combat reaction: %d", tmp.groupCombatReaction);
            fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
        }
    }

    fread(&(record->data), subrecordHead->DataSize, 1, esm_file);
    log_subrecord_new(subrecordHead);
    log_debug("Data:");
    log_debug("Flags 1: 0x%02x", record->data.flags_1);
    log_debug("Flags 2: 0x%02x", record->data.flags_2);
    log_debug("Unused bytes: 0x%02x 0x%02x", record->data.unused[0], record->data.unused[1]);

    if (ftell(esm_file) >= end) {
        return (Record*)record;
    }

    // CNAM, RNAM, INAM, WMI1 are optional
    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    log_info("Current file pointer location: 0x%06x", ftell(esm_file));

    if (strncmp(subrecordHead->Type, "CNAM", 4) == 0) {
        fread(&(record->unused), sizeof(float), 1, esm_file);
        log_subrecord_new(subrecordHead);
        log_debug("Unused float value: %f", record->unused);
        fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    }
    if (strncmp(subrecordHead->Type, "RNAM", 4) == 0) {
        FACT_RankSubrecords tmp;

        while (strncmp(subrecordHead->Type, "RNAM", 4) == 0) {
            tmp.male   = NULL;
            tmp.female = NULL;
            log_subrecord_new(subrecordHead);

            fread(&tmp.rankNumber, sizeof(uint32_t), 1, esm_file);
            log_debug("Rank number: %d", tmp.rankNumber);
            log_debug("Current file pointer location: 0x%06x", ftell(esm_file));

            fread(subrecordHead, sizeof(Subrecord), 1, esm_file);

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

            fread(subrecordHead, sizeof(Subrecord), 1, esm_file);

            if (strncmp(subrecordHead->Type, "FNAM", 4)) {
                arrput(record->rank, tmp);
                continue;
            }

            log_debug("Current file pointer location: 0x%06x", ftell(esm_file));
            tmp.female = init_cstring_subrecord(esm_file, subrecordHead, "Female");

            fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
            if (strncmp(subrecordHead->Type, "INAM", 4) == 0) {
                fread(&tmp.insignia, sizeof(uint32_t), 1, esm_file);
                log_debug("Insignia: %u", tmp.insignia);
                fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
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
        fseek(esm_file, -sizeof(Subrecord), SEEK_CUR);
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

/*
 * HDPT record specific fields
 */
static char* modelFilenameHeaders[] = {
    "MODL",
    "MOD2",
    "MOD3",
    "MOD4"
};

static char* modelTexhashHeaders[] = {
    "MODT",
    "MO2T",
    "MO3T",
    "MO4T"
};

static char* modelAltTexHeaders[] = {
    "MODS",
    "MO2S",
    "MO3S",
    "MO4S"
};

static char* modelFGFlagsHeaders[] = {
    "MODD",
    "MOSD"
};

Record* init_HDPT(FILE* esm_file)
{
    MALLOC_WARN(HDPTRecord, record);
    Subrecord subheader;

    fread(&(record->base), sizeof(Record), 1, esm_file);
    log_record(&(record->base));
    uint32_t dataEnd = ftell(esm_file) + record->base.DataSize;

    fread(&subheader, sizeof(Subrecord), 1, esm_file);
    assert(strncmp(subheader.Type, "EDID", 4) == 0);
    record->editorID = init_cstring_subrecord(esm_file, &subheader, "Editor ID");

    fread(&subheader, sizeof(Subrecord), 1, esm_file);
    assert(strncmp(subheader.Type, "FULL", 4) == 0);
    record->name = init_cstring_subrecord(esm_file, &subheader, "Name");

    AlternateTexture altTex;

    uint8_t fnameInd   = 0;
    uint8_t texHashInd = 0;
    uint8_t altTexInd  = 0;
    uint8_t flagsInd   = 0;

    fread(&subheader, sizeof(Subrecord), 1, esm_file);
    while (strncmp(subheader.Type, "DATA", 4) && strncmp(subheader.Type, "HNAM", 4) && ftell(esm_file) < dataEnd) {
        if (strncmp(subheader.Type, modelFilenameHeaders[fnameInd], 4) == 0) {
            assert(fnameInd <= 4);

            sds fname = init_cstring_subrecord(esm_file, &subheader, "Model filename");
            arrput(record->modelData.filenames, fname);
            fnameInd++;
        } else if (strncmp(subheader.Type, modelTexhashHeaders[texHashInd], 4) == 0) {
            assert(texHashInd < 4);

            MALLOC_N_WARN(uint8_t, subheader.DataSize, hval);

            fread(hval, sizeof(uint8_t), subheader.DataSize, esm_file);
            arrput(record->modelData.textureHashes, hval);
            texHashInd++;
        } else if (strncmp(subheader.Type, modelAltTexHeaders[altTexInd], 4) == 0) {
            assert(altTexInd < 4);

            uint32_t numAltTex;
            fread(&numAltTex, sizeof(uint32_t), 1, esm_file);

            for (uint32_t i = 0; i < numAltTex; i++) {
                uint32_t nameLen;
                fread(&nameLen, sizeof(uint32_t), 1, esm_file);

                char* cstring = malloc(nameLen);
                fread(cstring, nameLen, 1, esm_file);
                altTex.name = sdsnewlen(cstring, nameLen);
                free(cstring);

                fread(&(altTex.newTexture), sizeof(formid), 1, esm_file);
                fread(&(altTex.index), sizeof(formid), 1, esm_file);
                arrput(record->modelData.alternateTextures, altTex);
            }
            altTexInd++;

        } else if (strncmp(subheader.Type, modelFGFlagsHeaders[flagsInd], 4) == 0) {
            assert(flagsInd < 2);

            fread(&(record->modelData.MODDFlags[flagsInd]), sizeof(uint8_t), 1, esm_file);
            flagsInd++;
        } else {
            log_fatal("Something is fishy");
            return NULL;
        }
        fread(&subheader, sizeof(Subrecord), 1, esm_file);
    };

    assert(strncmp(subheader.Type, "DATA", 4) == 0);
    fread(&(record->flag), sizeof(uint8_t), 1, esm_file);

    fread(&subheader, sizeof(Subrecord), 1, esm_file);

    while (strncmp(subheader.Type, "HNAM", 4) == 0) {
        fread(&(record->extraParts), sizeof(formid), 1, esm_file);
        fread(&subheader, sizeof(Subrecord), 1, esm_file);
    }
    fseek(esm_file, -sizeof(Subrecord), SEEK_CUR);

    return (Record*)record;
}

void free_HDPT(Record* record)
{
    HDPTRecord* hdpt = (HDPTRecord*)record;

    sdsfree(hdpt->editorID);
    sdsfree(hdpt->name);

    uint32_t len = arrlenu(hdpt->modelData.filenames);
    for (uint32_t i = 0; i < len; i++) {
        sdsfree(hdpt->modelData.filenames[i]);
    }
    arrfree(hdpt->modelData.filenames);

    len = arrlenu(hdpt->modelData.textureHashes);
    for (uint32_t i = 0; i < len; i++) {
        free(hdpt->modelData.textureHashes[i]);
    }
    arrfree(hdpt->modelData.textureHashes);

    len = arrlenu(hdpt->modelData.alternateTextures);
    for (uint32_t i = 0; i < len; i++) {
        sdsfree(hdpt->modelData.alternateTextures[i].name);
    }

    arrfree(hdpt->modelData.alternateTextures);

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
