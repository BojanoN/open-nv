#include "esm.h"
#include "init.h"
#include "util/memutils.h"
#include <string.h>

Esm* esmnew(const sds path)
{
    FILE* esm_file;
    esm_file = fopen(path, "rb");
    if (esm_file == NULL) {
        log_fatal("file %s does not exist", path);
        exit(1);
    }

    SAFE_MALLOC(Esm, ret);

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
