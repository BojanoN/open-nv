#include "records.h"

RecordInits* record_initializers = NULL;


Record* init_TES4(FILE* esm_file){
  TES4Record* ret = (TES4Record* )malloc(sizeof(TES4Record));
  Subrecord tmp;

  fread(&(ret->base), RECORD_SIZE, 1, esm_file);
  fread(&(ret->hedr), sizeof(HEDR), 1, esm_file);

  return (Record*) ret;
}



void init_functionmap(){
  shput(record_initializers, "TES4", init_TES4);
}

void free_functionmap(){
  shfree(record_initializers);
}
