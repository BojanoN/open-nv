#include "records.h"

DECLARE_FUNCTION_MAPS(Record);

Record* init_TES4(FILE* esm_file)
{
    TES4Record* ret = (TES4Record*)malloc(sizeof(TES4Record));
    Subrecord   tmp;

    fread(&(ret->base), RECORD_SIZE, 1, esm_file);
    fread(&(ret->hedr), sizeof(HEDR), 1, esm_file);
    fread(&(ret->cnam), sizeof(Subrecord), 1, esm_file);

    ret->cnam.author = (sds)malloc(ret->cnam.base.DataSize);
    fread(ret->cnam.author, ret->cnam.base.DataSize, 1, esm_file);

    log_record(&(ret->base));
    log_subrecord(&(ret->hedr));
    log_subrecord(&(ret->cnam));

    return (Record*)ret;
}

void Record_init_constructor_map()
{
    shput(Record_constructor_map, "TES4", init_TES4);
}

void Record_init_destructor_map()
{
}
