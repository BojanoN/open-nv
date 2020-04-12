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
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    log_record(&hdr);

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID, subheader, esm_file, "Editor id");

    switch ((record->editorID)[0]) {
    case 's': {
        SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "DATA", record->value.stringValue, subheader, esm_file, "Game setting string value:");
        break;
    }
    case 'f':
        SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "DATA", float, record->value.floatValue, subheader, esm_file, "Game setting float value: %f");
        break;
    default:
        SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "DATA", int32_t, record->value.intValue, subheader, esm_file, "Game setting integer value: %f");
        break;
    }

    return (Record*)record;
}

Record* init_TXST(FILE* esm_file)
{
    MALLOC_WARN(TXSTRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    log_record(&hdr);

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID, subheader, esm_file, "Editor id");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_STRUCT_SUBRECORD, "OBND", ObjectBounds,
        record->objectBounds, subheader, esm_file);
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "TX00", record->baseImage_transparency, subheader, esm_file, "Base image/Transparency");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "TX01", record->normalMap_specular, subheader, esm_file, "Normal map/Specular");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "TX02", record->environmentMapMask, subheader, esm_file, "Environment map mask");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "TX03", record->glowMap, subheader, esm_file, "Glow map");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "TX04", record->parallaxMap, subheader, esm_file, "Parallax map");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "TX05", record->environmentMap, subheader, esm_file, "Environment map");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_STRUCT_SUBRECORD, "DODT", DecalData,
        record->decalData, subheader, esm_file);
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_MAIN_SUBRECORD, "DNAM", uint16_t, record->flags, subheader, esm_file, "Flags: 0x%04x");

    return (Record*)record;
}

Record* init_GLOB(FILE* esm_file)
{
    MALLOC_WARN(GLOBRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    log_record(&hdr);

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID, subheader, esm_file, "Editor id");
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "FNAM", uint8_t, record->type, subreader, esm_file, "Type: %c");
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "FLTV", uint32_t, record->value.longValue, subreader, esm_file, "Value: 0x%08x");
    switch (record->type) {
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
    return (Record*)record;
}

Record* init_FACT(FILE* esm_file)
{
    MALLOC_WARN(FACTRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    log_record(&hdr);

    uint32_t end = ftell(esm_file) + hdr.DataSize;

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID,
        subheader, esm_file, "Editor id");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "FULL", record->name,
        subheader, esm_file, "Name");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_SUBRECORD_COLLECTION, "XNAM",
        FactionRaceEx, record->relations, subheader,
        esm_file);

    //What the fuck
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "DATA", 4) == 0);
    if (subheader.DataSize == 1) {
        fread(&(record->data), 1, 1, esm_file);
        record->data.flags_2 = 0;
    } else {
        fread(&(record->data), sizeof(FactionData), 1, esm_file);
    }
    log_subrecord(&subheader);
    log_FactionData(&(record->data));
    /*SUBRECORD_WITH_HEADER_READ(STRUCT_SUBRECORD, "DATA", FactionData,
                               record->data, subheader, esm_file);
    */
    if (ftell(esm_file) >= end) {
        return (Record*)record;
    }

    SUBRECORD_WITH_HEADER_READ(OPTIONAL_MAIN_SUBRECORD, "CNAM", float, record->unused, subheader, esm_file, "Unused value: %f");

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    if (strncmp(subheader.Type, "RNAM", 4) == 0) {
        FactionRank currentFactRank;

        while (strncmp(subheader.Type, "RNAM", 4) == 0) {
            currentFactRank.male     = NULL;
            currentFactRank.female   = NULL;
            currentFactRank.insignia = 0;
            MAIN_SUBRECORD("RNAM", int32_t, currentFactRank.rankNumber, subheader, esm_file, "Rank number: %d");
            if (ftell(esm_file) >= end) {
                arrput(record->ranks, currentFactRank);
                return (Record*)record;
            }
            SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "MNAM", currentFactRank.male, subheader, esm_file, "Male");
            if (ftell(esm_file) >= end) {
                arrput(record->ranks, currentFactRank);
                return (Record*)record;
            }
            SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "FNAM", currentFactRank.female, subheader, esm_file, "Female");
            if (ftell(esm_file) >= end) {
                arrput(record->ranks, currentFactRank);
                return (Record*)record;
            }
            SUBRECORD_WITH_HEADER_READ(OPTIONAL_MAIN_SUBRECORD, "INAM", uint32_t, currentFactRank.insignia, subheader, esm_file, "Insignia: %u");
            arrput(record->ranks, currentFactRank);

            fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
        }
        fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);

    } else {
        assert((strncmp("MNAM", subheader.Type, 4)) != 0 && (strncmp("FNAM", subheader.Type, 4)) != 0 && (strncmp("INAM", subheader.Type, 4)) != 0);
        fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);
    }

    if (ftell(esm_file) >= end) {
        return (Record*)record;
    }

    SUBRECORD_WITH_HEADER_READ(OPTIONAL_MAIN_SUBRECORD, "WMI1", formid, record->reputation, subheader, esm_file, "Reputation: %d");
    assert(ftell(esm_file) == end);

    return (Record*)record;
}

Record* init_MICN(FILE* esm_file)
{
    MALLOC_WARN(MICNRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    log_record(&hdr);

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID, subheader, esm_file, "Editor id");
    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "ICON", record->largeIconFilename, subheader, esm_file, "Large icon filename");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "MICO", record->smallIconFilename, subheader, esm_file, "Small icon filename");
    return (Record*)record;
}

Record* init_CLAS(FILE* esm_file)
{
    MALLOC_WARN(CLASRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    log_record(&hdr);

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID, subheader, esm_file, "Editor id");
    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "FULL", record->name, subheader, esm_file, "Name");
    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "DESC", record->description, subheader, esm_file, "Description");
    SUBRECORD_WITH_HEADER_READ(STRUCT_SUBRECORD, "DATA",
        ClassData, record->data, subheader,
        esm_file);
    SUBRECORD_WITH_HEADER_READ(STRUCT_SUBRECORD, "ATTR",
        ClassAttributes, record->attributes, subheader,
        esm_file);
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

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID, subheader, esm_file, "Editor id");
    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "FULL", record->name, subheader, esm_file, "Name");

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

    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "DATA", uint8_t, record->flag, subreader, esm_file, "Flag: 0x%02x");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_MAIN_SUBRECORD_COLLECTION, "HNAM", formid, record->extraParts, subheader, esm_file, "Extra part: %d");

    return (Record*)record;
}

Record* init_HAIR(FILE* esm_file)
{
    MALLOC_WARN(HAIRRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID, subheader, esm_file, "Editor id");
    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "FULL", record->name, subheader, esm_file, "Name");

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

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "ICON", record->texture, subheader, esm_file, "Texture");
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "DATA", uint8_t, record->flag, subreader, esm_file, "Flag: 0x%02x");

    return (Record*)record;
}

Record* init_EYES(FILE* esm_file)
{
    MALLOC_WARN(EYESRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID, subheader, esm_file, "Editor id");
    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "FULL", record->name, subheader, esm_file, "Name");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "ICON", record->texture, subheader, esm_file, "Texture");
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "DATA", uint8_t, record->flag, subreader, esm_file, "Flag: 0x%02x");

    return (Record*)record;
}

Record* init_RACE(FILE* esm_file)
{
    MALLOC_WARN(RACERecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);
    log_record(&hdr);
    int readCounter = 0;

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID, subheader, esm_file, "Editor id");
    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "FULL", record->name, subheader, esm_file, "Name");
    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "DESC", record->description, subheader, esm_file, "Description");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_SUBRECORD_COLLECTION, "XNAM", FactionRaceEx, record->relations, subheader, esm_file);
    SUBRECORD_WITH_HEADER_READ(STRUCT_SUBRECORD, "DATA",
        RaceData, record->raceData, subheader,
        esm_file);
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_MAIN_SUBRECORD, "ONAM", formid, record->older, subheader, esm_file, "Older: %d");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_MAIN_SUBRECORD, "YNAM", formid, record->younger, subheader, esm_file, "Younger: %d");
    SUBRECORD_WITH_HEADER_READ(MARKER_SUBRECORD, "NAM2", subheader);
    SUBRECORD_WITH_HEADER_READ(STRUCT_SUBRECORD, "VTCK",
        RaceVoices, record->voices, subheader,
        esm_file);
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_STRUCT_SUBRECORD, "DNAM",
        RaceDefaultHairStyle, record->defaultHair, subheader,
        esm_file);
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_STRUCT_SUBRECORD, "CNAM",
        RaceDefaultHairColor, record->defaultHairColor, subheader,
        esm_file);
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "PNAM", float, record->faceGenMainClamp, subreader, esm_file, "FaceGen main clamp: %d");
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "UNAM", float, record->faceGenFaceClamp, subreader, esm_file, "FaceGen face clamp: %f");
    SUBRECORD_WITH_HEADER_READ(DUMMY_SUBRECORD, "ATTR", subheader);
    SUBRECORD_WITH_HEADER_READ(MARKER_SUBRECORD, "NAM0", subheader);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "MNAM", 4) == 0);
    log_subrecord(&subheader);
    record->maleHeadParts = init_ModelPartCollection(esm_file);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FNAM", 4) == 0);
    log_subrecord(&subheader);
    record->femaleHeadParts = init_ModelPartCollection(esm_file);

    SUBRECORD_WITH_HEADER_READ(MARKER_SUBRECORD, "NAM1", subheader);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "MNAM", 4) == 0);
    log_subrecord(&subheader);
    record->maleBodyParts = init_ModelPartCollection(esm_file);

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    assert(strncmp(subheader.Type, "FNAM", 4) == 0);
    log_subrecord(&subheader);
    record->femaleBodyParts = init_ModelPartCollection(esm_file);

    SUBRECORD_WITH_HEADER_READ(ARRAY_SUBRECORD, "HNAM", formid, record->hair, subheader, esm_file, "%d", "Hair");
    SUBRECORD_WITH_HEADER_READ(ARRAY_SUBRECORD, "ENAM", formid, record->eyes, subheader, esm_file, "%d", "Eyes");
    SUBRECORD_WITH_HEADER_READ(MARKER_SUBRECORD, "MNAM", subheader);
    SUBRECORD_WITH_HEADER_READ(ARRAY_SUBRECORD, "FGGS", uint8_t, record->maleFaceGenGeomSymm, subheader, esm_file, "%d", "Male FaceGen geometry (symmetric)");
    SUBRECORD_WITH_HEADER_READ(ARRAY_SUBRECORD, "FGGA", uint8_t, record->maleFaceGenGeomAsymm, subheader, esm_file, "%d", "Male FaceGen geometry (asymmetric)");
    SUBRECORD_WITH_HEADER_READ(ARRAY_SUBRECORD, "FGTS", uint8_t, record->maleFaceGenTexSymm, subheader, esm_file, "%d", "Male FaceGen texture (symmetric)");
    SUBRECORD_WITH_HEADER_READ(DUMMY_SUBRECORD, "SNAM", subheader);
    SUBRECORD_WITH_HEADER_READ(MARKER_SUBRECORD, "FNAM", subheader);
    SUBRECORD_WITH_HEADER_READ(ARRAY_SUBRECORD, "FGGS", uint8_t, record->femaleFaceGenGeomSymm, subheader, esm_file, "%d", "Female FaceGen geometry (symmetric)");
    SUBRECORD_WITH_HEADER_READ(ARRAY_SUBRECORD, "FGGA", uint8_t, record->femaleFaceGenGeomAsymm, subheader, esm_file, "%d", "Female FaceGen geometry (asymmetric)");
    SUBRECORD_WITH_HEADER_READ(ARRAY_SUBRECORD, "FGTS", uint8_t, record->femaleFaceGenTexSymm, subheader, esm_file, "%d", "Female FaceGen texture (symmetric)");
    SUBRECORD_WITH_HEADER_READ(DUMMY_SUBRECORD, "SNAM", subheader);

    return (Record*)record;
}

Record* init_SOUN(FILE* esm_file)
{
    MALLOC_WARN(SOUNRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    int      readCounter = 0;
    uint32_t end         = ftell(esm_file) + hdr.DataSize;

    log_record(&hdr);

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID,
        subheader, esm_file, "Editor ID");

    SUBRECORD_WITH_HEADER_READ(OPTIONAL_STRUCT_SUBRECORD, "OBND", ObjectBounds,
        record->objectBounds, subheader, esm_file);

    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "FNAM",
        record->soundFilename, subheader, esm_file, "Sound filename");

    SUBRECORD_WITH_HEADER_READ(OPTIONAL_MAIN_SUBRECORD, "RNAM", uint8_t,
        record->objectBounds, subheader, esm_file, "Random change percentage: %d");

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    if (strncmp(subheader.Type, "SNDD", 4) == 0) {
        STRUCT_SUBRECORD("SNDD", SoundData, record->soundData, subheader, esm_file);

    } else {
        STRUCT_SUBRECORD("SNDX", SoundEx, record->soundData, subheader, esm_file);

        if (ftell(esm_file) >= end) {
            return (Record*)record;
        }

        SUBRECORD_WITH_HEADER_READ(FIXED_LENGTH_ARRAY_SUBRECORD, "ANAM", int16_t, 5, record->soundData.attenuationPoints, subheader, esm_file, "%d", "Attenuation point");
        if (ftell(esm_file) >= end) {
            return (Record*)record;
        }

        SUBRECORD_WITH_HEADER_READ(OPTIONAL_MAIN_SUBRECORD, "GNAM", int16_t,
            record->soundData.reverbAttenuationControl, subheader, esm_file, "Reverb attenuation control: %d");
        if (ftell(esm_file) >= end) {
            return (Record*)record;
        }

        SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "HNAM", int32_t,
            record->soundData.priority, subheader, esm_file, "Priority: %d");
    }

    return (Record*)record;
}

Record* init_ASPC(FILE* esm_file)
{
    MALLOC_WARN(ASPCRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    log_record(&hdr);

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID,
        subheader, esm_file, "Editor ID");
    SUBRECORD_WITH_HEADER_READ(STRUCT_SUBRECORD, "OBND", ObjectBounds,
        record->objectBounds, subheader, esm_file);
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "SNAM", formid,
        record->dawn_default, subheader, esm_file,
        "Dawn: %d");
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "SNAM", formid,
        record->afternoon, subheader, esm_file,
        "Afternoon: %d");
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "SNAM", formid, record->dusk,
        subheader, esm_file, "Dusk: %d");
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "SNAM", formid, record->night,
        subheader, esm_file, "Night: %d");
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "SNAM", formid, record->walla,
        subheader, esm_file, "Walla: %d");
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "WNAM", uint32_t,
        record->wallaTriggerCount, subheader, esm_file,
        "Walla trigger count: %d");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_MAIN_SUBRECORD, "RDAT", formid,
        record->regionSound, subheader, esm_file,
        "Use sound from region: %d");
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "ANAM", uint32_t,
        record->environmentType, subheader, esm_file,
        "Environment type: %d");
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "INAM", uint32_t,
        record->isInterior, subheader, esm_file,
        "Is interior: %d");

    return (Record*)record;
}

Record* init_MGEF(FILE* esm_file)
{
    MALLOC_WARN(MGEFRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);
    log_record(&hdr);

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID, subheader, esm_file,
        "Editor ID");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "FULL", record->name, subheader, esm_file, "Name");
    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "DESC", record->description, subheader, esm_file,
        "Description");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "ICON", record->largeIconFilename, subheader, esm_file,
        "Large icon filename");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "MICO", record->smallIconFilename, subheader, esm_file,
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
    SUBRECORD_WITH_HEADER_READ(STRUCT_SUBRECORD, "DATA", MagicEffectData, record->magicEffectData, subheader, esm_file);

    return (Record*)record;
}

Record* init_SCPT(FILE* esm_file)
{
    MALLOC_WARN(SCPTRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);
    log_record(&hdr);
    uint32_t end         = ftell(esm_file) + hdr.DataSize;
    int      readCounter = 0;

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID, subheader, esm_file,
        "Editor ID");
    SUBRECORD_WITH_HEADER_READ(STRUCT_SUBRECORD, "SCHR",
        ScriptHeader, record->scriptHeader, subheader,
        esm_file);
    log_ScriptHeader(&(record->scriptHeader));
    SUBRECORD_WITH_HEADER_READ(ARRAY_SUBRECORD, "SCDA",
        uint8_t, record->compiledSource, subheader,
        esm_file, "0x%02x ", "Compiled source");
    SUBRECORD_WITH_HEADER_READ(ARRAY_SUBRECORD, "SCTX", char, record->scriptSource, subheader, esm_file, "%c", "Source");

    if (ftell(esm_file) >= end) {
        return (Record*)record;
    }

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    while (strncmp("SLSD", subheader.Type, 4) == 0) {
        LocalVariable currentLocalVariable;
        log_debug("Local variable:");
        STRUCT_SUBRECORD("SLSD", LocalVariableData, currentLocalVariable.data, subheader, esm_file);
        SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "SCVR", currentLocalVariable.name, subheader, esm_file, "Local variable name");
        arrput(record->localVariables, currentLocalVariable);

        if (ftell(esm_file) >= end) {
            return (Record*)record;
        }
        fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    }

    while ((strncmp("SCRO", subheader.Type, 4) == 0) || (strncmp("SCRV", subheader.Type, 4) == 0)) {
        ScriptReference reference;
        log_subrecord(&subheader);
        readCounter = fread(&(reference.referenceValue), sizeof(uint32_t), 1, esm_file);
        assert(readCounter == 1);
        if ((strncmp("SCRV", subheader.Type, 4) == 0)) {
            reference.type = VARIABLE_REFERENCE;
            log_debug("Variable reference: %d", reference.referenceValue.variableReference);
        } else {
            reference.type = OBJECT_REFERENCE;
            log_debug("Object reference: %d", reference.referenceValue.objectReference);
        }
        arrput(record->references, reference);

        if (ftell(esm_file) >= end) {
            return (Record*)record;
        }
        fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    }

    return (Record*)record;
}

void free_SCPT(Record* record)
{
    SCPTRecord* script = (SCPTRecord*)record;
    sdsfree(script->editorID);
    free(script->compiledSource);
    free(script->scriptSource);
    for (int i = 0; i < arrlen(script->localVariables); ++i) {
        sdsfree(script->localVariables[i].name);
    }
    arrfree(script->localVariables);
    arrfree(script->references);
    free(script);
}

Record* init_LTEX(FILE* esm_file)
{
    MALLOC_WARN(LTEXRecord, record);
    RecordHeader    hdr;
    SubrecordHeader subheader;

    fread(&hdr, sizeof(RecordHeader), 1, esm_file);
    FILL_BASE_RECORD_INFO(hdr, record);

    log_record(&hdr);

    SUBRECORD_WITH_HEADER_READ(CSTRING_SUBRECORD, "EDID", record->editorID, subheader, esm_file, "Editor id");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "ICON", record->largeIcon, subheader, esm_file, "Editor id");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_CSTRING_SUBRECORD, "MICO", record->smallIcon, subheader, esm_file, "Editor id");
    SUBRECORD_WITH_HEADER_READ(OPTIONAL_MAIN_SUBRECORD, "TNAM", formid, record->texture, subheader, esm_file, "Texture: %d");
    SUBRECORD_WITH_HEADER_READ(STRUCT_SUBRECORD, "HNAM", HavokData, record->havok, subheader, esm_file);
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD, "SNAM", uint8_t, record->textureSpecularExponent, subheader, esm_file, "Texture Specular Exponent: %d");
    SUBRECORD_WITH_HEADER_READ(MAIN_SUBRECORD_COLLECTION, "GNAM", formid, record->grass, subheader, esm_file, "Grass: %d");

    return (Record*)record;
}

void free_LTEX(Record* record)
{
    LTEXRecord* ltex = (LTEXRecord*)record;

    sdsfree(ltex->editorID);

    if (ltex->largeIcon != NULL) {
        sdsfree(ltex->largeIcon);
    }
    if (ltex->smallIcon != NULL) {
        sdsfree(ltex->smallIcon);
    }

    arrfree(ltex->grass);
    free(ltex);
}

void free_MGEF(Record* record)
{
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

void free_ASPC(Record* record)
{
    ASPCRecord* acousticSpace = (ASPCRecord*)record;
    sdsfree(acousticSpace->editorID);
    free(acousticSpace);
}

void free_SOUN(Record* record)
{
    SOUNRecord* sound = (SOUNRecord*)record;

    sdsfree(sound->editorID);
    if (sound->soundFilename != NULL) {
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

    uint32_t len = arrlenu(fact->ranks);
    for (uint32_t i = 0; i < len; i++) {
        if (fact->ranks[i].male != NULL) {
            sdsfree(fact->ranks[i].male);
        }
        if (fact->ranks[i].female != NULL) {
            sdsfree(fact->ranks[i].female);
        }
    }
    arrfree(fact->ranks);

    sdsfree(fact->editorID);
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
    arrfree(hdpt->extraParts);

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
    if ((gmst_record->editorID)[0] == 's') {
        sdsfree(gmst_record->value.stringValue);
    }
    sdsfree(gmst_record->editorID);
    free(record);
}

void free_MICN(Record* record)
{
    MICNRecord* rec = (MICNRecord*)record;

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
    sdsfree(txst->editorID);

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
    sdsfree(glob_record->editorID);
    free(glob_record);
}

void free_CLAS(Record* record)
{
    CLASRecord* clas = (CLASRecord*)record;

    sdsfree(clas->editorID);
    sdsfree(clas->description);
    sdsfree(clas->name);

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
    ADD_CONSTRUCTOR(Record, "SCPT", init_SCPT);
    ADD_CONSTRUCTOR(Record, "LTEX", init_LTEX);
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
    ADD_DESTRUCTOR(Record, "SCPT", free_SCPT);
    ADD_DESTRUCTOR(Record, "LTEX", free_LTEX);
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
