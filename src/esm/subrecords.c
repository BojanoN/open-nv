#include "subrecords.h"
#include "util/memutils.h"

DECLARE_FUNCTION_MAPS(Subrecord);

Subrecord* create_HEDR(FILE* esm_file) {
  SAFE_MALLOC(HEDR, ret);

  fread(ret, sizeof(HEDR), 1, esm_file);
  log_subrecord(ret);

  return (Subrecord*)ret;
}

Subrecord* create_CNAM(FILE* esm_file) {
  SAFE_MALLOC(CNAM, ret);

  fread(ret, sizeof(Subrecord), 1, esm_file);

  char* tmp = (char*)malloc(ret->base.DataSize);
  fread(tmp, ret->base.DataSize, 1, esm_file);

  ret->author = sdsnewlen(tmp, ret->base.DataSize);
  free(tmp);

  log_subrecord(ret);
  log_debug("Author: %s", ret->author);

  return (Subrecord*)ret;
}

Subrecord* create_EDID(FILE* esm_file) {
  SAFE_MALLOC(EDID, ret);

  fread(ret, sizeof(Subrecord), 1, esm_file);

  char* tmp = (char*)malloc(ret->base.DataSize);
  fread(tmp, ret->base.DataSize, 1, esm_file);
  ret->editorId = sdsnewlen(tmp, ret->base.DataSize);
  free(tmp);

  log_subrecord(ret);
  log_debug("Editor ID: %s", ret->editorId);

  return (Subrecord*)ret;
}

Subrecord* create_DATA(FILE* esm_file) {
  SAFE_MALLOC(DATA, ret);

  fread(ret, sizeof(Subrecord), 1, esm_file);

  char* tmp = (char*)malloc(ret->base.DataSize);
  fread(tmp, ret->base.DataSize, 1, esm_file);
  ret->value = sdsnewlen(tmp, ret->base.DataSize);
  free(tmp);

  log_subrecord(ret);
  log_debug("Value as string: %s", ret->value);

  if (ret->base.DataSize == 4) {
    log_debug("Value as integer: %d",
              (uint32_t)((ret->value)[0] | (ret->value)[1] << 8 |
                         (ret->value)[2] << 16 | (ret->value)[3] << 24));
    log_debug("Value as float: %f",
              (float)((ret->value)[0] | (ret->value)[1] << 8 |
                      (ret->value)[2] << 16 | (ret->value)[3] << 24));
  }

  return (Subrecord*)ret;
}

Subrecord* create_OBND(FILE* esm_file) {
  SAFE_MALLOC(OBND, ret);

  fread(ret, sizeof(OBND), 1, esm_file);
  log_subrecord(ret);

  return (Subrecord*)ret;
}

Subrecord* create_TX00(FILE* esm_file) {
    SAFE_MALLOC(TX00, ret);

    fread(ret, sizeof(Subrecord), 1, esm_file);

    char* tmp = (char*)malloc(ret->base.DataSize);
    fread(tmp, ret->base.DataSize, 1, esm_file);
    ret->baseImage_transparency = sdsnewlen(tmp, ret->base.DataSize);
    free(tmp);

    log_subrecord(ret);

    return (Subrecord*)ret;
}

Subrecord* create_TX01(FILE* esm_file) {
    SAFE_MALLOC(TX01, ret);

    fread(ret, sizeof(Subrecord), 1, esm_file);

    char* tmp = (char*)malloc(ret->base.DataSize);
    fread(tmp, ret->base.DataSize, 1, esm_file);
    ret->normalMap_specular = sdsnewlen(tmp, ret->base.DataSize);
    free(tmp);

    log_subrecord(ret);

    return (Subrecord*)ret;
}

Subrecord* create_TX02(FILE* esm_file) {
    SAFE_MALLOC(TX02, ret);

    fread(ret, sizeof(Subrecord), 1, esm_file);

    char* tmp = (char*)malloc(ret->base.DataSize);
    fread(tmp, ret->base.DataSize, 1, esm_file);
    ret->environmentMapMask = sdsnewlen(tmp, ret->base.DataSize);
    free(tmp);

    log_subrecord(ret);

    return (Subrecord*)ret;
}

Subrecord* create_TX03(FILE* esm_file) {
    SAFE_MALLOC(TX03, ret);

    fread(ret, sizeof(Subrecord), 1, esm_file);

    char* tmp = (char*)malloc(ret->base.DataSize);
    fread(tmp, ret->base.DataSize, 1, esm_file);
    ret->glowMap = sdsnewlen(tmp, ret->base.DataSize);
    free(tmp);

    log_subrecord(ret);

    return (Subrecord*)ret;
}

Subrecord* create_TX04(FILE* esm_file) {
    SAFE_MALLOC(TX04, ret);

    fread(ret, sizeof(Subrecord), 1, esm_file);

    char* tmp = (char*)malloc(ret->base.DataSize);
    fread(tmp, ret->base.DataSize, 1, esm_file);
    ret->parallaxMap = sdsnewlen(tmp, ret->base.DataSize);
    free(tmp);

    log_subrecord(ret);

    return (Subrecord*)ret;
}

Subrecord* create_TX05(FILE* esm_file) {
    SAFE_MALLOC(TX05, ret);

    fread(ret, sizeof(Subrecord), 1, esm_file);

    char* tmp = (char*)malloc(ret->base.DataSize);
    fread(tmp, ret->base.DataSize, 1, esm_file);
    ret->environmentMap = sdsnewlen(tmp, ret->base.DataSize);
    free(tmp);

    log_subrecord(ret);

    return (Subrecord*)ret;
}

Subrecord* create_DODT(FILE* esm_file) {
  SAFE_MALLOC(DODT, ret);

  fread(ret, sizeof(DODT), 1, esm_file);
  log_subrecord(ret);

  return (Subrecord*)ret;
}

Subrecord* create_DNAM(FILE* esm_file) {
  SAFE_MALLOC(DNAM, ret);

  fread(ret, sizeof(DNAM), 1, esm_file);
  log_subrecord(ret);

  return (Subrecord*)ret;
}


void free_HEDR(Subrecord* record) {
  HEDR* tmp = (HEDR*)record;
  free(tmp);
}

void free_CNAM(Subrecord* record) {
  CNAM* tmp = (CNAM*)record;
  sdsfree(tmp->author);
  free(tmp);
}

void free_EDID(Subrecord* record) {
  EDID* tmp = (EDID*)record;
  sdsfree(tmp->editorId);
  free(tmp);
}

void free_DATA(Subrecord* record) {
  DATA* tmp = (DATA*)record;
  sdsfree(tmp->value);
  free(tmp);
}

void free_OBND(Subrecord* record) {
  OBND* tmp = (OBND*)record;
  free(tmp);
}

void free_TX00(Subrecord* record) {
  TX00* tmp = (TX00*)record;
  sdsfree(tmp->baseImage_transparency);
  free(tmp);
}

void free_TX01(Subrecord* record) {
  TX01* tmp = (TX01*)record;
  sdsfree(tmp->normalMap_specular);
  free(tmp);
}

void free_TX02(Subrecord* record) {
  TX02* tmp = (TX02*)record;
  sdsfree(tmp->environmentMapMask);
  free(tmp);
}

void free_TX03(Subrecord* record) {
  TX03* tmp = (TX03*)record;
  sdsfree(tmp->glowMap);
  free(tmp);
}

void free_TX04(Subrecord* record) {
  TX04* tmp = (TX04*)record;
  sdsfree(tmp->parallaxMap);
  free(tmp);
}

void free_TX05(Subrecord* record) {
  TX05* tmp = (TX05*)record;
  sdsfree(tmp->environmentMap);
  free(tmp);
}

void free_DODT(Subrecord* record) {
  DODT* tmp = (DODT*)record;
  free(tmp);
}

void free_DNAM(Subrecord* record) {
  DNAM* tmp = (DNAM*)record;
  free(tmp);
}

void Subrecord_init_constructor_map() {
  ADD_CONSTRUCTOR(Subrecord, "HEDR", create_HEDR);
  ADD_CONSTRUCTOR(Subrecord, "CNAM", create_CNAM);
  ADD_CONSTRUCTOR(Subrecord, "EDID", create_EDID);
  ADD_CONSTRUCTOR(Subrecord, "DATA", create_DATA);
  ADD_CONSTRUCTOR(Subrecord, "OBND", create_OBND);
  ADD_CONSTRUCTOR(Subrecord, "TX00", create_TX00);
  ADD_CONSTRUCTOR(Subrecord, "TX01", create_TX01);
  ADD_CONSTRUCTOR(Subrecord, "TX02", create_TX02);
  ADD_CONSTRUCTOR(Subrecord, "TX03", create_TX03);
  ADD_CONSTRUCTOR(Subrecord, "TX04", create_TX04);
  ADD_CONSTRUCTOR(Subrecord, "TX05", create_TX05);
  ADD_CONSTRUCTOR(Subrecord, "DODT", create_DODT);
  ADD_CONSTRUCTOR(Subrecord, "DNAM", create_DNAM);
}

void Subrecord_init_destructor_map() {
  ADD_DESTRUCTOR(Subrecord, "HEDR", free_HEDR);
  ADD_DESTRUCTOR(Subrecord, "CNAM", free_CNAM);
  ADD_DESTRUCTOR(Subrecord, "EDID", free_EDID);
  ADD_DESTRUCTOR(Subrecord, "DATA", free_DATA);
  ADD_DESTRUCTOR(Subrecord, "OBND", free_OBND);
  ADD_DESTRUCTOR(Subrecord, "TX00", free_TX00);
  ADD_DESTRUCTOR(Subrecord, "TX01", free_TX01);
  ADD_DESTRUCTOR(Subrecord, "TX02", free_TX02);
  ADD_DESTRUCTOR(Subrecord, "TX03", free_TX03);
  ADD_DESTRUCTOR(Subrecord, "TX04", free_TX04);
  ADD_DESTRUCTOR(Subrecord, "TX05", free_TX05);
  ADD_DESTRUCTOR(Subrecord, "DODT", free_DODT);
  ADD_DESTRUCTOR(Subrecord, "DNAM", free_DNAM);
}
