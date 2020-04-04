#include "records.h"
#include "util/memutils.h"

DECLARE_FUNCTION_MAPS(Record);

FETCH_CONSTRUCTOR_MAP(Subrecord);
FETCH_DESTRUCTOR_MAP(Subrecord);

Record* init_TES4(FILE* esm_file)
{
    SAFE_MALLOC(TES4Record, ret);

    fread(&(ret->base), RECORD_SIZE, 1, esm_file);

    SubrecordConstructor* func  = GET_CONSTRUCTOR(Subrecord, "HEDR");
    HEDR*                 tmp_h = (HEDR*)func(esm_file);

    if (tmp_h == NULL) {
        return NULL;
    }
    ret->hedr = tmp_h;

    func        = GET_CONSTRUCTOR(Subrecord, "CNAM");
    CNAM* tmp_c = (CNAM*)func(esm_file);

    if (tmp_c == NULL) {
        return NULL;
    }
    ret->cnam = tmp_c;

    log_record(&(ret->base));

    return (Record*)ret;
}

void free_TES4(Record* record)
{
    TES4Record*          tmp  = (TES4Record*)record;
    SubrecordDestructor* func = GET_DESTRUCTOR(Subrecord, "HEDR");
    func((Subrecord*)tmp->hedr);

    func = GET_DESTRUCTOR(Subrecord, "CNAM");
    func((Subrecord*)tmp->cnam);

    free(record);
}

void Record_init_constructor_map()
{
    ADD_CONSTRUCTOR(Record, "TES4", init_TES4);
}

void Record_init_destructor_map()
{
    ADD_DESTRUCTOR(Record, "TES4", free_TES4);
}

Record* recordnew(FILE* f, sds type)
{
    Record*            ret  = NULL;
    RecordConstructor* func = GET_CONSTRUCTOR(Record, type);

    if (func == NULL) {
        log_warn("Record type %s not yet implemented.", type);
        return NULL;
    }

    ret = func(f);
    return ret;
}
void recordfree(Record* record)
{
    sds               type = sdsnewlen(record->Type, 4);
    RecordDestructor* func = GET_DESTRUCTOR(Record, type);
    func(record);
    sdsfree(type);
}
