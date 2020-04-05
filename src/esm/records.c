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

Record* init_GMST(FILE* esm_file) {
  SAFE_MALLOC(GMSTRecord, ret);

  fread(&(ret->base), RECORD_SIZE, 1, esm_file);

  SubrecordConstructor* constructor = GET_CONSTRUCTOR(Subrecord, "EDID");
  EDID* tmp_edid = (EDID*)constructor(esm_file);

  if (tmp_edid == NULL) {
    return NULL;
  }

  ret->edid = tmp_edid;

  constructor = GET_CONSTRUCTOR(Subrecord, "DATA");
  DATA* tmp_data = (DATA*)constructor(esm_file);

  if (tmp_data == NULL) {
    return NULL;
  }
  ret->data = tmp_data;

  log_record(&(ret->base));

  return (Record*)ret;
}

Record* init_TXST(FILE* esm_file) {
  SAFE_MALLOC(TXSTRecord, ret);
  fread(&(ret->base), RECORD_SIZE, 1, esm_file);

  SubrecordConstructor* constructor = GET_CONSTRUCTOR(Subrecord, "EDID");
  EDID* tmp_edid = (EDID*)constructor(esm_file);

  if (tmp_edid == NULL) {
    return NULL;
  }
  ret->edid = tmp_edid;

  constructor = GET_CONSTRUCTOR(Subrecord, "OBND");
  OBND* tmp_obnd = (OBND*)constructor(esm_file);

  if (tmp_obnd == NULL) {
    return NULL;
  }
  ret->obnd = tmp_obnd;

  constructor = GET_CONSTRUCTOR(Subrecord, "TX00");
  TX00* tmp_tx00 = (TX00*)constructor(esm_file);

  if (tmp_tx00 == NULL) {
    return NULL;
  }
  ret->tx00 = tmp_tx00;

  constructor = GET_CONSTRUCTOR(Subrecord, "TX01");
  TX01* tmp_tx01 = (TX01*)constructor(esm_file);

  if (tmp_tx01 == NULL) {
    return NULL;
  }
  ret->tx01 = tmp_tx01;

  constructor = GET_CONSTRUCTOR(Subrecord, "TX02");
  TX02* tmp_tx02 = (TX02*)constructor(esm_file);

  if (tmp_tx02 == NULL) {
    return NULL;
  }
  ret->tx02 = tmp_tx02;

  constructor = GET_CONSTRUCTOR(Subrecord, "TX03");
  TX03* tmp_tx03 = (TX03*)constructor(esm_file);

  if (tmp_tx03 == NULL) {
    return NULL;
  }
  ret->tx03 = tmp_tx03;

  constructor = GET_CONSTRUCTOR(Subrecord, "TX04");
  TX04* tmp_tx04 = (TX04*)constructor(esm_file);

  if (tmp_tx04 == NULL) {
    return NULL;
  }
  ret->tx04 = tmp_tx04;

  constructor = GET_CONSTRUCTOR(Subrecord, "TX05");
  TX05* tmp_tx05 = (TX05*)constructor(esm_file);

  if (tmp_tx05 == NULL) {
    return NULL;
  }
  ret->tx05 = tmp_tx05;

  constructor = GET_CONSTRUCTOR(Subrecord, "DODT");
  DODT* tmp_dodt = (DODT*)constructor(esm_file);

  if (tmp_dodt == NULL) {
    return NULL;
  }
  ret->dodt = tmp_dodt;

  constructor = GET_CONSTRUCTOR(Subrecord, "DNAM");
  DNAM* tmp_dnam = (DNAM*)constructor(esm_file);

  if (tmp_dnam == NULL) {
    return NULL;
  }
  ret->dnam = tmp_dnam;

  log_record(&(ret->base));

  return (Record*)ret;
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
  GMSTRecord* tmp = (GMSTRecord*)record;
  SubrecordDestructor* destructor = GET_DESTRUCTOR(Subrecord, "EDID");
  destructor((Subrecord*)tmp->edid);

  destructor = GET_DESTRUCTOR(Subrecord, "DATA");
  destructor((Subrecord*)tmp->data);

  free(record);
}

void free_TXST(Record* record) {
  TXSTRecord* tmp = (TXSTRecord*)record;
  SubrecordDestructor* destructor = GET_DESTRUCTOR(Subrecord, "EDID");
  destructor((Subrecord*)tmp->edid);

  destructor = GET_DESTRUCTOR(Subrecord, "OBND");
  destructor((Subrecord*)tmp->obnd);

  destructor = GET_DESTRUCTOR(Subrecord, "TX00");
  destructor((Subrecord*)tmp->tx00);

  destructor = GET_DESTRUCTOR(Subrecord, "TX01");
  destructor((Subrecord*)tmp->tx01);

  destructor = GET_DESTRUCTOR(Subrecord, "TX02");
  destructor((Subrecord*)tmp->tx02);

  destructor = GET_DESTRUCTOR(Subrecord, "TX03");
  destructor((Subrecord*)tmp->tx03);

  destructor = GET_DESTRUCTOR(Subrecord, "TX04");
  destructor((Subrecord*)tmp->tx04);

  destructor = GET_DESTRUCTOR(Subrecord, "TX05");
  destructor((Subrecord*)tmp->tx05);

  destructor = GET_DESTRUCTOR(Subrecord, "DODT");
  destructor((Subrecord*)tmp->dodt);

  destructor = GET_DESTRUCTOR(Subrecord, "DNAM");
  destructor((Subrecord*)tmp->dnam);

  free(record);
}

void Record_init_constructor_map() {
  ADD_CONSTRUCTOR(Record, "TES4", init_TES4);
  ADD_CONSTRUCTOR(Record, "GMST", init_GMST);
  ADD_CONSTRUCTOR(Record, "TXST", init_TXST);
}

void Record_init_destructor_map() {
  ADD_DESTRUCTOR(Record, "TES4", free_TES4);
  ADD_DESTRUCTOR(Record, "GMST", free_GMST);
  ADD_DESTRUCTOR(Record, "TXST", free_TXST);
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
