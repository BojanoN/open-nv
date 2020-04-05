#pragma once

#include "esm.h"
#include "subrecords.h"

typedef struct {
  Record base;
  HEDR* hedr;
  CNAM* cnam;
} TES4Record;

/*
    Game setting
*/
typedef struct {
  Record base;
  sds  editorId;
  union {
    int32_t intValue;
    float floatValue;
    sds   stringValue;
  } value;
} GMSTRecord;

/*
    Texture set
*/
typedef struct {
  Record base;
  sds  editorId;
  OBNDSubrecord objectBounds;
  sds baseImage_transparency;
  sds normalMap_specular;
  sds environmentMapMask;
  sds glowMap;
  sds parallaxMap;
  sds environmentMap;
  DODTSubrecord decalData;
  uint16_t flags;
} TXSTRecord;