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

    MALLOC_WARN(Esm, ret);

    ret->records = NULL;
    ret->groups  = NULL;
    sds type     = sdsnewlen("init", 4);

    uint32_t cnt = 0;
    while (fread(type, 4, 1, esm_file)) {
        fseek(esm_file, -4, SEEK_CUR);
        log_debug("Read type: %s", type);

        if (strcmp(type, GROUP_TYPE) == 0) {
            Group* g = groupnew(esm_file);
            if (g != NULL) {
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
            if (r != NULL) {
                hmput(ret->records, r->ID, r);
            } else {
                log_fatal("Fatal error during ESM file parsing");

                esmfree(ret);
                sdsfree(type);
                fclose(esm_file);
                return NULL;
            }
        }

        log_debug("Current file pointer location: 0x%06x", ftell(esm_file));
        //na 0x0A95E zavrsava grupa GMST
        //na 0x021F4B zavrsava grupa TXST
        //na 0x0224FC zavrsava grupa MICN
        //na 0x025DB0 zavrsava grupa GLOB
        //na 0x027F5D zavrsava grupa CLAS
        //na 0x03C22E zavrsava grupa FACT
        //na 0x03F003 zavrsava grupa HDPT
        //na 0x043108 zavrsava grupa HAIR
        //na 0x0435D9 zavrsava grupa EYES
        //na 0x05f528 zavrsava grupa RACE
        //na 0x0DA255 zavrsava grupa SOUN
        //na 0x0DE72E zavrsava grupa ASPC
        //na 0x0E9ED9 zavrsava grupa MGEF
        //na 0x54CC6A zavrsava grupa SCPT
        if (ftell(esm_file) >= 0x54CC66)
            break;
        cnt++;
    }

#ifdef DEBUG
    uint32_t totalRead = ftell(esm_file);
    uint32_t size;
    fseek(esm_file, 0, SEEK_END);
    size = ftell(esm_file);
    log_info("%.3f percent of file parsed", ((double)(totalRead) / size) * 100);
#endif

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
