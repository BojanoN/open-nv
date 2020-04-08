#pragma once

#include "esm.h"

typedef struct __attribute__((packed)) {
    Subrecord base;
    float     version;
    uint32_t  numRecords;
    uint32_t  nextObjectId;
} HEDR;

typedef struct __attribute__((packed)) {
    Subrecord base;
    sds       author;
} CNAM;

/*
  Object bounds
*/
typedef struct __attribute__((packed)) {
    int16_t x1;
    int16_t y1;
    int16_t z1;
    int16_t x2;
    int16_t y2;
    int16_t z2;
} OBNDSubrecord;

/*
  Decal data
*/
typedef struct __attribute__((packed)) {
    float   minWidth;
    float   maxWidth;
    float   minHeight;
    float   maxHeight;
    float   depth;
    float   shininess;
    float   parallaxScale;
    uint8_t parallaxPasses;
    uint8_t flags;
    uint8_t unused[2];
    uint8_t color[4];
} DODTSubrecord;

/*
 * NPC stats
 */
typedef struct __attribute__((packed)) {
    // 4 skill total
    int32_t  tagSkills[4];
    uint32_t flags;
    uint32_t services;
    int8_t   teaches;
    uint8_t  maxTrainingLevel;
    uint8_t  unused[2];
} DATASubrecord;

/*
 * Attributes
 */
typedef struct __attribute__((packed)) {
    uint8_t strength;
    uint8_t perception;
    uint8_t endurance;
    uint8_t charisma;
    uint8_t intelligence;
    uint8_t agility;
    uint8_t luck;
} ATTRSubrecord;

typedef struct __attribute__((packed)) {
    formid  faction;
    int32_t modifier;

    /*
    0 - NEUTRAL
    1 - ENEMY
    2 - ALLY
    3 - FRIEND
  */
    uint32_t groupCombatReaction;
} XNAMSubrecord;

typedef struct __attribute__((packed)) {
    /*
    0x01 - hidden
    0x02 - evil
    0x04 - special combat
    */
    uint8_t flags_1;
    /*
    0x01 - track crime
    0x02 - allow sell
    */
    uint8_t flags_2;
    uint8_t unused[2];
} FACT_DATASubrecord;

typedef struct __attribute__((packed)) {
    int32_t  rankNumber;
    sds      male;
    sds      female;
    uint32_t insignia;
} FACT_RankSubrecords;

typedef struct {
    sds name;
    /*
     * FormID of a TXST record.
     */
    formid   newTexture;
    uint32_t index;
} AlternateTexture;

typedef struct __attribute__((packed)) {
    /*
     * Array of filenames.
     * Matches subrecords MODL, MOD2, MOD3, MOD4
     */
    sds* filenames;
    /*
     * MODB
     */
    uint32_t unknown;
    /*
     * MODT, MO2T, MO3T, MO4T
     */
    uint8_t** textureHashes;
    /*
     * MODS, MO2S, MO3S, MO4S
     */
    AlternateTexture* alternateTextures;
    /*
     * MODD, MOSD
     */
    uint8_t MODDFlags[2];
} ModelDataSubrecord;
