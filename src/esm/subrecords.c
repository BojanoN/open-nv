#include "subrecords.h"
#include "util/memutils.h"

DECLARE_FUNCTION_MAPS(Subrecord);

sds init_cstring_subrecord(FILE* esm_file, SubrecordHeader* subrecordHead, const char* loggingName)
{
    char* cstring = malloc(subrecordHead->DataSize);
    fread(cstring, subrecordHead->DataSize, 1, esm_file);
    sds subrecord = sdsnewlen(cstring, subrecordHead->DataSize);
    free(cstring);
    log_subrecord(subrecordHead);
    log_debug("%s: %s", loggingName, subrecord);
    return subrecord;
}

Subrecord* create_HEDR(FILE* esm_file)
{
    MALLOC_WARN(HEDR, ret);
    SubrecordHeader hdr;
    fread(&hdr, sizeof(SubrecordHeader), 1, esm_file);
    fread(ret, sizeof(HEDR), 1, esm_file);
    log_subrecord(&hdr);

    return (Subrecord*)ret;
}

Subrecord* create_CNAM(FILE* esm_file)
{
    MALLOC_WARN(CNAM, ret);
    SubrecordHeader hdr;
    fread(&hdr, sizeof(SubrecordHeader), 1, esm_file);

    char* tmp = (char*)malloc(hdr.DataSize);
    fread(tmp, hdr.DataSize, 1, esm_file);

    ret->author = sdsnewlen(tmp, hdr.DataSize);
    free(tmp);

    log_subrecord(&hdr);
    log_debug("Author: %s", ret->author);

    return (Subrecord*)ret;
}

Subrecord* create_OBND(FILE* esm_file)
{
    MALLOC_WARN(OBNDSubrecord, subrecord);

    fread(subrecord, sizeof(OBNDSubrecord), 1, esm_file);
    log_debug("Object bounds:");
    log_debug("X 1: %d", subrecord->x1);
    log_debug("Y 1: %d", subrecord->y1);
    log_debug("Z 1: %d", subrecord->z1);
    log_debug("X 2: %d", subrecord->x2);
    log_debug("Y 2: %d", subrecord->y2);
    log_debug("Z 2: %d", subrecord->z2);

    return (Subrecord*)subrecord;
}

Subrecord* create_DODT(FILE* esm_file)
{
    MALLOC_WARN(DODTSubrecord, subrecord);

    fread(subrecord, sizeof(DODTSubrecord), 1, esm_file);
    log_debug("Decal data:");
    log_debug("Min width: %f", subrecord->minWidth);
    log_debug("Max width: %f", subrecord->maxWidth);
    log_debug("Min height: %f", subrecord->minHeight);
    log_debug("Max height: %f", subrecord->maxHeight);
    log_debug("Depth: %f", subrecord->depth);
    log_debug("Shininess: %f", subrecord->shininess);
    log_debug("Parallax scale: %f", subrecord->parallaxScale);
    log_debug("Parallax passes: %d", subrecord->parallaxPasses);
    log_debug("Flags: 0x%02x", subrecord->flags);
    log_debug("Unused bytes: 0x%02x 0x%02x", subrecord->unused[0], subrecord->unused[1]);
    log_debug("Color: 0x%02x 0x%02x 0x%02x 0x%02x", subrecord->color[0], subrecord->color[1], subrecord->color[2], subrecord->color[3]);

    return (Subrecord*)subrecord;
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

Subrecord* create_ModelData(FILE* esm_file)
{
    MALLOC_WARN(ModelDataSubrecord, modelData);
    SubrecordHeader subheader;

    uint8_t fnameInd   = 0;
    uint8_t texHashInd = 0;
    uint8_t altTexInd  = 0;
    uint8_t flagsInd   = 0;

    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    AlternateTexture altTex;
    while (strncmp(subheader.Type, "DATA", 4) && strncmp(subheader.Type, "HNAM", 4)) {
        if (strncmp(subheader.Type, modelFilenameHeaders[fnameInd], 4) == 0) {
            assert(fnameInd <= 4);

            sds fname = init_cstring_subrecord(esm_file, &subheader, "Model filename");
            arrput(modelData->filenames, fname);
            fnameInd++;
        } else if (strncmp(subheader.Type, modelTexhashHeaders[texHashInd], 4) == 0) {
            assert(texHashInd < 4);

            MALLOC_N_WARN(uint8_t, subheader.DataSize, hval);

            fread(hval, sizeof(uint8_t), subheader.DataSize, esm_file);
            arrput(modelData->textureHashes, hval);
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
                arrput(modelData->alternateTextures, altTex);
            }
            altTexInd++;

        } else if (strncmp(subheader.Type, modelFGFlagsHeaders[flagsInd], 4) == 0) {
            assert(flagsInd < 2);

            fread(&(modelData->MODDFlags[flagsInd]), sizeof(uint8_t), 1, esm_file);
            flagsInd++;
        } else {
            log_fatal("Something is fishy");
            return NULL;
        }
        fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
    }
    fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);

    return (Subrecord*)modelData;
}

void free_ModelData(Subrecord* record)
{
    ModelDataSubrecord* modelData = (ModelDataSubrecord*)record;

    uint32_t len = arrlenu(modelData->filenames);
    for (uint32_t i = 0; i < len; i++) {
        sdsfree(modelData->filenames[i]);
    }
    arrfree(modelData->filenames);

    len = arrlenu(modelData->textureHashes);
    for (uint32_t i = 0; i < len; i++) {
        free(modelData->textureHashes[i]);
    }
    arrfree(modelData->textureHashes);

    len = arrlenu(modelData->alternateTextures);
    for (uint32_t i = 0; i < len; i++) {
        sdsfree(modelData->alternateTextures[i].name);
    }

    arrfree(modelData->alternateTextures);
}

void free_HEDR(Subrecord* record)
{
    HEDR* tmp = (HEDR*)record;
    free(tmp);
}

void free_CNAM(Subrecord* record)
{
    CNAM* tmp = (CNAM*)record;
    sdsfree(tmp->author);
    free(tmp);
}

void Subrecord_init_constructor_map()
{
    ADD_CONSTRUCTOR(Subrecord, "HEDR", create_HEDR);
    ADD_CONSTRUCTOR(Subrecord, "CNAM", create_CNAM);
    ADD_CONSTRUCTOR(Subrecord, "OBND", create_OBND);
    ADD_CONSTRUCTOR(Subrecord, "DODT", create_DODT);
    ADD_CONSTRUCTOR(Subrecord, "MODL", create_ModelData);
}

void Subrecord_init_destructor_map()
{
    ADD_DESTRUCTOR(Subrecord, "HEDR", free_HEDR);
    ADD_DESTRUCTOR(Subrecord, "CNAM", free_CNAM);
    ADD_DESTRUCTOR(Subrecord, "MODL", free_ModelData);
}
