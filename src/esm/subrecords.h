#pragma once

#include "esm.h"

typedef struct __attribute__((packed)) {
  Subrecord base;
  float version;
  uint32_t numRecords;
  uint32_t nextObjectId;
} HEDR;

typedef struct __attribute__((packed)) {
  Subrecord base;
  sds author;
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
  float minWidth;
  float maxWidth;
  float minHeight;
  float maxHeight;
  float depth;
  float shininess;
  float parallaxScale;
  uint8_t parallaxPasses;
  uint8_t flags;
  uint8_t unused[2];
  uint8_t color[4];
} DODTSubrecord;
