#include "records.h"
#include "util/memutils.h"

DECLARE_FUNCTION_MAPS(Record);

FETCH_CONSTRUCTOR_MAP(Subrecord);
FETCH_DESTRUCTOR_MAP(Subrecord);

Record* init_TES4(FILE* esm_file) {
  SAFE_MALLOC(TES4Record, ret);

  fread(&(ret->base), RECORD_SIZE, 1, esm_file);

  SubrecordConstructor* func = GET_CONSTRUCTOR(Subrecord, "HEDR");
  HEDR* tmp_h = (HEDR*)func(esm_file);

  if (tmp_h == NULL) {
    return NULL;
  }
  ret->hedr = tmp_h;

  func = GET_CONSTRUCTOR(Subrecord, "CNAM");
  CNAM* tmp_c = (CNAM*)func(esm_file);

  if (tmp_c == NULL) {
    return NULL;
  }
  ret->cnam = tmp_c;

  log_record(&(ret->base));

  return (Record*)ret;
}

sds init_cstring_subrecord(FILE* esm_file, Subrecord* subrecordHead, const char* loggingName) {
  char* cstring = malloc(subrecordHead->DataSize);
  fread(cstring, subrecordHead->DataSize, 1, esm_file);
  sds subrecord = sdsnewlen(cstring, subrecordHead->DataSize);
  free(cstring);
  log_subrecord_new(subrecordHead);
  log_debug("%s: %s", loggingName, subrecord);
  return subrecord;
}

Record* init_GMST(FILE* esm_file) {
  SAFE_MALLOC(GMSTRecord, record);

  fread(&(record->base), RECORD_SIZE, 1, esm_file);
  //EDID
  SAFE_MALLOC(Subrecord, subrecordHead);
  fread(subrecordHead, sizeof(Subrecord), 1, esm_file);

  record->editorId = init_cstring_subrecord(esm_file, subrecordHead, "Editor ID");

  //DATA
  fread(subrecordHead, sizeof(Subrecord), 1, esm_file);

  size_t dataSize = subrecordHead->DataSize;
  char* cstring = malloc(dataSize);
  fread(cstring, dataSize, 1, esm_file);
  record->value.stringValue = sdsnewlen(cstring, dataSize);
  free(cstring);
  log_subrecord_new(subrecordHead);

  switch((record->editorId)[0]) {
    case 's':
      log_debug("Game setting string value: %s", record->value.stringValue);
      break;
    case 'f':
      log_debug("Game setting float value: %f", record->value.floatValue);
      break;
    default:
      log_debug("Game setting integer value: %d", record->value.intValue);
      break;
  }

  log_record(&(record->base));

  return (Record*)record;
}

Record* init_TXST(FILE* esm_file) {
  SAFE_MALLOC(TXSTRecord, record);
  fread(&(record->base), RECORD_SIZE, 1, esm_file);

  //EDID
  SAFE_MALLOC(Subrecord, subrecordHead);
  fread(subrecordHead, sizeof(Subrecord), 1, esm_file);

  record->editorId = init_cstring_subrecord(esm_file, subrecordHead, "Editor ID");

  fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
  sds type = sdsnewlen(subrecordHead->Type, 4);

  SubrecordConstructor* constructor = NULL;
  //OBND
  if(strcmp(type, "OBND") == 0) {
    constructor = GET_CONSTRUCTOR(Subrecord, "OBND");
    OBNDSubrecord* objectBounds = (OBNDSubrecord*)constructor(esm_file); 
    record->objectBounds = *objectBounds;
    free(objectBounds);
    log_subrecord_new(subrecordHead);
    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    type = sdsnewlen(subrecordHead->Type, 4);
  }

  //TX00
  if(strcmp(type, "TX00") == 0) {
    record->baseImage_transparency = init_cstring_subrecord(esm_file, subrecordHead, "Base image/Transparency");
    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    type = sdsnewlen(subrecordHead->Type, 4);
  } else {
    record->baseImage_transparency = NULL;
  }

  //TX01
  if(strcmp(type, "TX01") == 0) {
    record->normalMap_specular = init_cstring_subrecord(esm_file, subrecordHead, "Normal map/Specular");
    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    type = sdsnewlen(subrecordHead->Type, 4);
  } else {
    record->normalMap_specular = NULL;
  }

  //TX02
  if(strcmp(type, "TX02") == 0) {
    record->environmentMapMask = init_cstring_subrecord(esm_file, subrecordHead, "Environment map mask");
    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    type = sdsnewlen(subrecordHead->Type, 4);
  } else {
    record->environmentMapMask = NULL;
  }

  //TX03
  if(strcmp(type, "TX03") == 0) {
    record->glowMap = init_cstring_subrecord(esm_file, subrecordHead, "Glow map");
    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    type = sdsnewlen(subrecordHead->Type, 4);
  } else {
    record->glowMap = NULL;
  }

  //TX04
  if(strcmp(type, "TX04") == 0) {
    record->parallaxMap = init_cstring_subrecord(esm_file, subrecordHead, "Parallax map");
    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    type = sdsnewlen(subrecordHead->Type, 4);
  } else {
    record->parallaxMap = NULL;
  }

  //TX05
  if(strcmp(type, "TX05") == 0) {
    record->environmentMap = init_cstring_subrecord(esm_file, subrecordHead, "Environment map");
    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    type = sdsnewlen(subrecordHead->Type, 4);
  } else {
    record->environmentMap = NULL;
  }

  //DODT
  if(strcmp(type, "DODT") == 0) {
    constructor = GET_CONSTRUCTOR(Subrecord, "DODT");
    DODTSubrecord* decalData = (DODTSubrecord*)constructor(esm_file); 
    record->decalData = *decalData;
    log_subrecord_new(subrecordHead);
    fread(subrecordHead, sizeof(Subrecord), 1, esm_file);
    type = sdsnewlen(subrecordHead->Type, 4);
  }

  //DNAM
  if(strcmp(type, "DNAM") == 0) {
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

Record* init_GLOB(FILE* esm_file) {
  SAFE_MALLOC(GLOBRecord, record);

  fread(&(record->base), RECORD_SIZE, 1, esm_file);

  SAFE_MALLOC(Subrecord, subrecordHead);
  fread(subrecordHead, sizeof(Subrecord), 1, esm_file);

  record->editorId = init_cstring_subrecord(esm_file, subrecordHead, "Editor ID");
  fread(subrecordHead, sizeof(Subrecord), 1, esm_file);

  uint8_t fnam;
  fread(&fnam, sizeof(uint8_t), 1, esm_file);
  record->type = fnam;
  log_subrecord_new(subrecordHead);
  log_debug("Type: %c", record->type);

  uint32_t value;
  fread(&value, sizeof(uint32_t), 1, esm_file);
  record->value.longValue = value;
  log_subrecord_new(subrecordHead);

  switch(fnam) {
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

void free_TES4(Record* record) {
  TES4Record* tmp = (TES4Record*)record;
  SubrecordDestructor* func = GET_DESTRUCTOR(Subrecord, "HEDR");
  func((Subrecord*)tmp->hedr);

  func = GET_DESTRUCTOR(Subrecord, "CNAM");
  func((Subrecord*)tmp->cnam);

  free(record);
}

void free_GMST(Record* record) {
  GMSTRecord* gmst_record = (GMSTRecord*)record;
  if((gmst_record->editorId)[0] == 's') {
    sdsfree(gmst_record->value.stringValue);
  }
  sdsfree(gmst_record->editorId);
  free(record);
}

void free_TXST(Record* record) {
  TXSTRecord* txst = (TXSTRecord*)record;
  sdsfree(txst->editorId);

  if(txst->baseImage_transparency != NULL) {
    sdsfree(txst->baseImage_transparency);
  }
  if(txst->normalMap_specular != NULL) {
    sdsfree(txst->normalMap_specular);
  }
  if(txst->environmentMapMask != NULL) {
    sdsfree(txst->environmentMapMask);
  }
  if(txst->glowMap != NULL) {
    sdsfree(txst->glowMap);
  }
  if(txst->parallaxMap != NULL) {
    sdsfree(txst->parallaxMap);
  }
  if(txst->environmentMap != NULL) {
    sdsfree(txst->environmentMap);
  }

  free(txst);
}

void free_GLOB(Record* record) {
  GLOBRecord* glob_record = (GLOBRecord*)record;
  sdsfree(glob_record->editorId);
  free(glob_record);
}

void Record_init_constructor_map() {
  ADD_CONSTRUCTOR(Record, "TES4", init_TES4);
  ADD_CONSTRUCTOR(Record, "GMST", init_GMST);
  ADD_CONSTRUCTOR(Record, "TXST", init_TXST);
  ADD_CONSTRUCTOR(Record, "GLOB", init_GLOB);
}

void Record_init_destructor_map() {
  ADD_DESTRUCTOR(Record, "TES4", free_TES4);
  ADD_DESTRUCTOR(Record, "GMST", free_GMST);
  ADD_DESTRUCTOR(Record, "TXST", free_TXST);
  ADD_DESTRUCTOR(Record, "GLOB", free_GLOB);
}

Record* recordnew(FILE* f, sds type) {
  Record* ret = NULL;
  RecordConstructor* func = GET_CONSTRUCTOR(Record, type);

  if (func == NULL) {
    log_warn("Record type %s not yet implemented.", type);
    return NULL;
  }

  ret = func(f);
  return ret;
}
void recordfree(Record* record) {
  sds type = sdsnewlen(record->Type, 4);
  RecordDestructor* func = GET_DESTRUCTOR(Record, type);
  func(record);
  sdsfree(type);
}
