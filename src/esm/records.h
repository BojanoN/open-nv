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
  EDID* edid;
  DATA* data;
} GMSTRecord;

/*
    Texture set
*/
typedef struct {
  Record base;
  EDID* edid;
  OBND* obnd;
  TX00* tx00;
  TX01* tx01;
  TX02* tx02;
  TX03* tx03;
  TX04* tx04;
  TX05* tx05;
  DODT* dodt;
  DNAM* dnam;
} TXSTRecord;