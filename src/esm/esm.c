#include "esm.h"
#include "init.h"
#include <string.h>

FETCH_CONSTRUCTOR_MAP(Record);
FETCH_DESTRUCTOR_MAP(Record);


Esm* esmnew(const sds path)
{
    FILE* esm_file;
    esm_file = fopen(path, "rb");
    if (esm_file == NULL) {
        log_fatal("file %s does not exist", path);
        exit(1);
    }

    Esm* ret     = (Esm*)malloc(sizeof(Esm));
    ret->records = NULL;
    sds type     = sdsnewlen("init", 4);

    while (fread(type, 4, 1, esm_file)) {
        fseek(esm_file, -4, SEEK_CUR);
        log_info("Read type: %s", type);
        if (strcmp(type, GROUP_TYPE) == 0) {
            // Load group
        } else {
            Record* r = recordnew(esm_file, type);
            if (r) {
                hmput(ret->records, r->ID, r);
            } else {
                log_fatal("Fatal error during ESM file parsing");
                return NULL;
            }
        }
        break;
    }
    sdsfree(type);
    fclose(esm_file);
    return ret;
}
void esmfree(Esm* esm)
{
    for (uint32_t i = 0; i < hmlenu(esm->records); i++) {
        recordfree(esm->records[i].value);
    }
    hmfree(esm->records);
    free(esm);
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
