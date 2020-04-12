#include "subrecords.h"
#include "util/memutils.h"

DECLARE_FUNCTION_MAPS(Subrecord);

/*
 * Ctors and dtors.
 */

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

void log_ObjectBounds(ObjectBounds* subrecord) {
    log_debug("Object bounds:");
    log_debug("X 1: %d", subrecord->x1);
    log_debug("Y 1: %d", subrecord->y1);
    log_debug("Z 1: %d", subrecord->z1);
    log_debug("X 2: %d", subrecord->x2);
    log_debug("Y 2: %d", subrecord->y2);
    log_debug("Z 2: %d", subrecord->z2);
}

void log_OBND(ObjectBounds* subrecord) {
    log_debug("Object bounds:");
    log_debug("X 1: %d", subrecord->x1);
    log_debug("Y 1: %d", subrecord->y1);
    log_debug("Z 1: %d", subrecord->z1);
    log_debug("X 2: %d", subrecord->x2);
    log_debug("Y 2: %d", subrecord->y2);
    log_debug("Z 2: %d", subrecord->z2);
}


void log_DecalData(DecalData* subrecord) {
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
}

void log_FactionRaceEx(FactionRaceEx* subrecord) {
    log_debug("Relation:");
    log_debug("Faction: %d", subrecord->faction);
    log_debug("Modifier: %d", subrecord->modifier);
    log_debug("Group combat reaction: %d", subrecord->groupCombatReaction);
}

void log_ClassData(ClassData* data) {
    log_debug("Class data:");
    for(int i = 0; i < 4; i++) {
        log_debug("Tag skill %d: %d", i, data->tagSkills[i]);
    }
    log_debug("Flags: 0x%08x", data->flags);
    log_debug("Services: 0x%08x", data->services);
    log_debug("Teaches: %d", data->teaches);
    log_debug("Maxmimum training level: %d", data->maxTrainingLevel);
    log_debug("Unused values: 0x%02x 0x%02x", data->unused[0], data->unused[1]);
}

void log_ClassAttributes(ClassAttributes* attr) {
    log_debug("Class attributes:");
    log_debug("Strength: %d", attr->strength);
    log_debug("Perception: %d", attr->perception);
    log_debug("Endurance: %d", attr->endurance);
    log_debug("Charisma: %d", attr->charisma);
    log_debug("Intelligence: %d", attr->intelligence);
    log_debug("Agility: %d", attr->agility);
    log_debug("Luck: %d", attr->luck);
}

void log_FactionData(FactionData* data) {
    log_debug("Faction data:");
    log_debug("Flags 1: 0x%02x", data->flags_1);
    log_debug("Flags 2: 0x%02x", data->flags_2);
    log_debug("Unused values: 0x%02x 0x%02x", data->unused[0], data->unused[1]);
}

void log_RaceData(RaceData* subrecord) {
    log_debug("Race data:");
    for(int i = 0; i < 7; i++) {
        log_debug("Skill boost %d: Skill: %d, Boost: %d", i, subrecord->skillBoosts[i].skill, subrecord->skillBoosts[i].boost);
    }
    log_debug("Unused: 0x%02x 0x%02x", subrecord->padding[0], subrecord->padding[1]);
    log_debug("Male height: %f", subrecord->maleHeight);
    log_debug("Female height: %f", subrecord->femaleHeight);
    log_debug("Male weight: %f", subrecord->maleWeight);
    log_debug("Female weight: %f", subrecord->femaleWeight);
    log_debug("Flags: 0x%08x", subrecord->flags);
}

void log_RaceVoices(RaceVoices* voices) {
    log_debug("Race voices:");
    log_debug("Male voice: %d", voices->male);
    log_debug("Female voice: %d", voices->female);
}

void log_RaceDefaultHairStyle(RaceDefaultHairStyle* subrecord) {
    log_debug("Default hair style:");
    log_debug("Male default hair style: %d", subrecord->male);
    log_debug("Female default hair style: %d", subrecord->female);
}

void log_RaceDefaultHairColor(RaceDefaultHairColor* subrecord) {
    log_debug("Default hair color:");
    log_debug("Male default hair color: %d", subrecord->male);
    log_debug("Female default hair color: %d", subrecord->female);
}

void log_SNDD(SoundData* subrecord) {
  log_debug("Sound data:");
  log_debug("Minimum attenuation distance: 5x%d",
            subrecord->minimumAttenuationDistance);
  log_debug("Maximum attenuation distance: 100x%d",
            subrecord->maximumAttenuationDistance);
  log_debug("Frequency adjustment percentage: %d",
            subrecord->frequencyAdjustmentPercentage);
  log_debug("Unused value: %d", subrecord->unused);
  log_debug("Flags: 0x%08x", subrecord->flags);
  log_debug("Static attenuation cdB: %d", subrecord->staticAttenuationCdB);
  log_debug("Stop time: %d", subrecord->stopTime);
  log_debug("Start time: %d", subrecord->startTime);
  log_debug("Attenuation points: %d, %d, %d, %d, %d",
            subrecord->attenuationPoints[0], subrecord->attenuationPoints[1],
            subrecord->attenuationPoints[2], subrecord->attenuationPoints[3],
            subrecord->attenuationPoints[4]);
  log_debug("Reverb Attenuation Control: %d",
            subrecord->reverbAttenuationControl);
  log_debug("Priority: %d", subrecord->priority);
  log_debug("X: %d", subrecord->x);
  log_debug("Y: %d", subrecord->y);
}

void log_SoundData(SoundData* subrecord) {
  log_debug("Sound data:");
  log_debug("Minimum attenuation distance: 5x%d",
            subrecord->minimumAttenuationDistance);
  log_debug("Maximum attenuation distance: 100x%d",
            subrecord->maximumAttenuationDistance);
  log_debug("Frequency adjustment percentage: %d",
            subrecord->frequencyAdjustmentPercentage);
  log_debug("Unused value: %d", subrecord->unused);
  log_debug("Flags: 0x%08x", subrecord->flags);
  log_debug("Static attenuation cdB: %d", subrecord->staticAttenuationCdB);
  log_debug("Stop time: %d", subrecord->stopTime);
  log_debug("Start time: %d", subrecord->startTime);
  log_debug("Attenuation points: %d, %d, %d, %d, %d",
            subrecord->attenuationPoints[0], subrecord->attenuationPoints[1],
            subrecord->attenuationPoints[2], subrecord->attenuationPoints[3],
            subrecord->attenuationPoints[4]);
  log_debug("Reverb Attenuation Control: %d",
            subrecord->reverbAttenuationControl);
  log_debug("Priority: %d", subrecord->priority);
  log_debug("X: %d", subrecord->x);
  log_debug("Y: %d", subrecord->y);
}

void log_SoundEx(SoundData* subrecord) {
  log_debug("Sound data:");
  log_debug("Minimum attenuation distance: 5x%d",
            subrecord->minimumAttenuationDistance);
  log_debug("Maximum attenuation distance: 100x%d",
            subrecord->maximumAttenuationDistance);
  log_debug("Frequency adjustment percentage: %d",
            subrecord->frequencyAdjustmentPercentage);
  log_debug("Unused value: %d", subrecord->unused);
  log_debug("Flags: 0x%08x", subrecord->flags);
  log_debug("Static attenuation cdB: %d", subrecord->staticAttenuationCdB);
  log_debug("Stop time: %d", subrecord->stopTime);
  log_debug("Start time: %d", subrecord->startTime);
}

void log_MagicEffectData(MagicEffectData* subrecord) {
    log_debug("Magic effect data:");
    log_debug("Flags: %08x", subrecord->flags);
    log_debug("Base cost (unused): %f", subrecord->baseCost);
    log_debug("Associated item: %d", subrecord->associatedItem);
    log_debug("Magic school (unused): %d", subrecord->magicSchool);
    log_debug("Resistance type: %d", subrecord->resistanceType);
    log_debug("Unknown value: 0x%04x", subrecord->unused_1);
    log_debug("Unused bytes: 0x%02x 0x%02x", subrecord->unused_2[0], subrecord->unused_2[1]);
    log_debug("Light: %d", subrecord->light);
    log_debug("Projectile speed: %f", subrecord->projectileSpeed);
    log_debug("Effect shader: %d", subrecord->effectShader);
    log_debug("Object display shader: %d", subrecord->objectDisplayShader);
    log_debug("Effect sound: %d", subrecord->effectSound);
    log_debug("Bold sound: %d", subrecord->boldSound);
    log_debug("Hit sound: %d", subrecord->hitSound);
    log_debug("Area sound: %d", subrecord->areaSound);
    log_debug("Constant effect enchantment factor (unused): %f", subrecord->constantEffectEnchantmentFactor);
    log_debug("Constant effect barter factor (unused): %f", subrecord->constantEffectBarterFactor);
    log_debug("Archtype: %u", subrecord->archtype);
    log_debug("Actor value: %d", subrecord->actorValue);
}

void log_ScriptHeader(ScriptHeader* subrecord) {
    log_debug("Script header:");
    log_debug("Ref count: %u", subrecord->refCount);
    log_debug("CompiledSize: %u", subrecord->compiledSize);
    log_debug("Variable count: %u", subrecord->variableCount);
    log_debug("Type: 0x%04x", subrecord->type);
    log_debug("Flags: 0x%04x", subrecord->flags);
}   


void log_LocalVariableData(LocalVariableData* subrecord) {
    log_debug("Index: %d", subrecord->index);
    log_debug("Flags: 0x%02x", subrecord->flags);
}

void log_HavokData(HavokData* subrecord) {
    log_debug("Havok data:");
    log_debug("Material type: %d", subrecord->materialType);
    log_debug("Friction: %d", subrecord->friction);
    log_debug("Restitution: %d", subrecord->restitution);
}

Subrecord* create_OBND(FILE* esm_file) {
  MALLOC_WARN(ObjectBounds, subrecord);

  fread(subrecord, sizeof(ObjectBounds), 1, esm_file);
  log_debug("Object bounds:");
  log_debug("X 1: %d", subrecord->x1);
  log_debug("Y 1: %d", subrecord->y1);
  log_debug("Z 1: %d", subrecord->z1);
  log_debug("X 2: %d", subrecord->x2);
  log_debug("Y 2: %d", subrecord->y2);
  log_debug("Z 2: %d", subrecord->z2);

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
    while (!(strncmp(subheader.Type, modelFilenameHeaders[fnameInd], 4) && strncmp(subheader.Type, modelTexhashHeaders[texHashInd], 4) && strncmp(subheader.Type, modelAltTexHeaders[altTexInd], 4) && strncmp(subheader.Type, modelFGFlagsHeaders[flagsInd], 4))) {
        if (strncmp(subheader.Type, modelFilenameHeaders[fnameInd], 4) == 0) {
            assert(fnameInd <= 4);

            sds fname = init_cstring_subrecord(esm_file, &subheader, "Model filename");
            if (fname == NULL) {
                log_fatal("Error parsing model filename");
                return NULL;
            }

            //arrput(modelData->filenames, fname);
            //assert(modelData->filenames[arrlenu(modelData->filenames) + 1] == NULL);
            modelData->filenames[fnameInd] = fname;
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
    modelData->noFname = fnameInd;
    fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);

    return (Subrecord*)modelData;
}

void free_ModelData(Subrecord* record)
{
    ModelDataSubrecord* modelData = (ModelDataSubrecord*)record;

    for (uint32_t i = 0; i < modelData->noFname; i++) {
        if (modelData->filenames[i] != NULL)
            sdsfree(modelData->filenames[i]);
        else
            break;
    }
    //    arrfree(modelData->filenames);

    uint32_t len = arrlenu(modelData->textureHashes);
    for (uint32_t i = 0; i < len; i++) {
        free(modelData->textureHashes[i]);
    }
    arrfree(modelData->textureHashes);

    len = arrlenu(modelData->alternateTextures);
    for (uint32_t i = 0; i < len; i++) {
        sdsfree(modelData->alternateTextures[i].name);
    }
    arrfree(modelData->alternateTextures);

    free(modelData);
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
    ADD_CONSTRUCTOR(Subrecord, "MODL", create_ModelData);
}

void Subrecord_init_destructor_map()
{
    ADD_DESTRUCTOR(Subrecord, "HEDR", free_HEDR);
    ADD_DESTRUCTOR(Subrecord, "CNAM", free_CNAM);
    ADD_DESTRUCTOR(Subrecord, "MODL", free_ModelData);
}

/*
 * Specific init functions.
 */

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

ModelPart* init_ModelPartCollection(FILE* esm_file)
{
    SubrecordHeader subheader;
    ModelPart*      retArr = NULL;
    ModelPart       tmpMdl;
    fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);

    while (strncmp(subheader.Type, "INDX", 4) == 0) {
        tmpMdl.largeIcon = NULL;
        tmpMdl.smallIcon = NULL;
        tmpMdl.modelData = NULL;
        fread(&tmpMdl.index, sizeof(uint32_t), 1, esm_file);
        log_subrecord(&subheader);

        fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);

        while (strncmp(subheader.Type, "INDX", 4) && strncmp(subheader.Type, "HNAM", 4) && strncmp(subheader.Type, "FNAM", 4) && strncmp(subheader.Type, "NAM1", 4)) {
            if (strncmp(subheader.Type, "MODL", 4) == 0) {
                fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);
                tmpMdl.modelData = (ModelDataSubrecord*)create_ModelData(esm_file);
                if (tmpMdl.modelData == NULL) {
                    log_fatal("Error while parsing ModelPart");
                    return NULL;
                }
                fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
                log_subrecord(&subheader);
            } else if (strncmp(subheader.Type, "ICON", 4) == 0) {
                tmpMdl.largeIcon = init_cstring_subrecord(esm_file, &subheader, "Large icon filename");
                fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
                log_subrecord(&subheader);
            } else if (strncmp(subheader.Type, "MICO", 4) == 0) {
                tmpMdl.smallIcon = init_cstring_subrecord(esm_file, &subheader, "Small icon filename");
                fread(&subheader, sizeof(SubrecordHeader), 1, esm_file);
                log_subrecord(&subheader);
            } else {
                log_fatal("Something is fishy");
                return NULL;
            }
        }
        arrput(retArr, tmpMdl);
    }

    fseek(esm_file, -sizeof(SubrecordHeader), SEEK_CUR);

    return retArr;
}

void free_ModelPartCollection(ModelPart* collection)
{
    uint32_t len = arrlenu(collection);

    for (uint32_t i = 0; i < len; i++) {
        ModelPart tmp = collection[i];

        if (tmp.largeIcon != NULL) {
            sdsfree(tmp.largeIcon);
        }
        if (tmp.largeIcon != NULL) {
            sdsfree(tmp.smallIcon);
        }
        if (tmp.modelData != NULL) {
            free_ModelData((Subrecord*)tmp.modelData);
        }
    }
    arrfree(collection);
}
