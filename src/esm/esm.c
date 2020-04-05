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
    ret->groups  = NULL;
    sds type     = sdsnewlen("init", 4);

    uint32_t cnt = 0;
    while (fread(type, 4, 1, esm_file)) {
        fseek(esm_file, -4, SEEK_CUR);
        log_info("Read type: %s", type);

        if (strcmp(type, GROUP_TYPE) == 0) {
            Group* g = groupnew(esm_file);
            if (g) {
                arrput(ret->groups, g);
            } else {
                log_fatal("Fatal error during ESM file parsing");

                esmfree(ret);
                sdsfree(type);
                fclose(esm_file);
                return NULL;
            }
        } else {
            Record* r = recordnew(esm_file, type);
            if (r) {
                hmput(ret->records, r->ID, r);
            } else {
                log_fatal("Fatal error during ESM file parsing");

                esmfree(ret);
                sdsfree(type);
                fclose(esm_file);
                return NULL;
            }
        }

        log_debug("Current file pointer location: 0x%05x", ftell(esm_file));
        //na 0xA95E zavrsava grupa GMST
        //na 0x21F4B zavrsava grupa TXST
        if (ftell(esm_file) > 0x21F4B)
            break;
        cnt++;
    }
    log_info("Read %u records and groups", cnt);
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

    for (uint32_t i = 0; i < arrlenu(esm->groups); i++) {
        groupfree(esm->groups[i]);
    }
    arrfree(esm->groups);

    free(esm);
}
