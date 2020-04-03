#include "esm.h"
#include "util/byte_hash.h"
#include <string.h>

/*
 * Record constructors
 */
extern RecordInits * record_initializers;

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
    init_functionmap();
    sds type = sdsnewlen("init", 4);

    while (fread(type, 4, 1, esm_file)) {
        fseek(esm_file, -4, SEEK_CUR);
        log_info("Read type: %s", type);
        if (strcmp(type, GROUP_TYPE) == 0) {
            // Load group
        } else {
            Record* r = recordnew(esm_file, type);
            if(r){
              hmput(ret->records, r->ID, r);
            }
        }
        break;
    }
    free_functionmap();
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
    Record* ret = NULL;
    record_init* func = shget(record_initializers, type);

    if(func == NULL){
      log_warn("Record type %s not yet implemented.", type);
      return NULL;
    }

    ret = func(f);
    return ret;
}
void recordfree(Record* record)
{
    free(record);
    // TODO: free any internal containers
}
