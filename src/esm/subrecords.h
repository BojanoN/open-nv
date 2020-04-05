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

typedef struct __attribute__((packed)) {
  Subrecord base;
  sds editorId;
} EDID;

typedef struct __attribute__((packed)) {
  Subrecord base;
  sds value;
} DATA;

typedef struct __attribute__((packed)) {
  Subrecord base;
  int16_t x1;
  int16_t y1;
  int16_t z1;
  int16_t x2;
  int16_t y2;
  int16_t z2;
} OBND;

typedef struct __attribute__((packed)) {
  Subrecord base;
  sds baseImage_transparency;
} TX00;

typedef struct __attribute__((packed)) {
  Subrecord base;
  sds normalMap_specular;
} TX01;

typedef struct __attribute__((packed)) {
  Subrecord base;
  sds environmentMapMask;
} TX02;

typedef struct __attribute__((packed)) {
  Subrecord base;
  sds glowMap;
} TX03;

typedef struct __attribute__((packed)) {
  Subrecord base;
  sds parallaxMap;
} TX04;

typedef struct __attribute__((packed)) {
  Subrecord base;
  sds environmentMap;
} TX05;

typedef struct __attribute__((packed)) {
  Subrecord base;
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
} DODT;

typedef struct __attribute__((packed)) {
  Subrecord base;
  uint16_t flags;
} DNAM;

/*
typedef struct __attribute__((packed)) {
	DATA base;
	sds value;
} DATA_STRING;

typedef struct __attribute__((packed)) {
	DATA base;
	uint32_t value;
} DATA_INT32;

typedef struct __attribute__((packed)) {
	DATA base;
	float value;
} DATA_FLOAT;
*/