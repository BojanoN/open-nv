#include "esm/esm.h"
#include "logc/log.h"
#include "sds/sds.h"

#include <getopt.h>
#include <time.h>

#define FALLOUTNV_ESM_DEFAULT_PATH "./bin/esm/FalloutNV.esm"

/*
 * Record constructors/destructors
 */
FETCH_CONSTRUCTOR_MAP(Record);
FETCH_DESTRUCTOR_MAP(Record);

/*
 * Subrecord constructors/destructors
 */

FETCH_CONSTRUCTOR_MAP(Subrecord);
FETCH_DESTRUCTOR_MAP(Subrecord);

/*
 * Group constructors/destructors
 */

FETCH_CONSTRUCTOR_MAP(Group);
FETCH_DESTRUCTOR_MAP(Group);

void init(void)
{
    INIT_CONSTRUCTOR_MAP(Record);
    INIT_DESTRUCTOR_MAP(Record);

    INIT_CONSTRUCTOR_MAP(Subrecord);
    INIT_DESTRUCTOR_MAP(Subrecord);

    INIT_CONSTRUCTOR_MAP(Group);
    INIT_DESTRUCTOR_MAP(Group);
}

void cleanup(void)
{
    FREE_CONSTRUCTOR_MAP(Record);
    FREE_DESTRUCTOR_MAP(Record);

    FREE_CONSTRUCTOR_MAP(Subrecord);
    FREE_DESTRUCTOR_MAP(Subrecord);

    FREE_CONSTRUCTOR_MAP(Group);
    FREE_DESTRUCTOR_MAP(Group);
}

int main(int argc, char** argv)
{
#ifdef DEBUG
    log_set_level(LOG_DEBUG);
#else
    log_set_level(LOG_INFO);
#endif

    int c;
    sds path = sdsnew(FALLOUTNV_ESM_DEFAULT_PATH);

    while ((c = getopt(argc, argv, "f:")) != -1) {
        switch (c) {
        case 'f':
            sdsfree(path);
            path = sdsnew(optarg);
            break;
        default:
            break;
        }
    }

    init();

    log_info("loading esm file from %s", path);

    Esm*    esm;
    clock_t start = clock();
    esm           = esmnew(path);
    clock_t end   = clock();

    if (esm == NULL) {
        log_fatal("Failed to initialize openNV");
        sdsfree(path);
        cleanup();
        return 1;
    }
    log_info("Loaded %s in %f miliseconds", path, (double)(end - start) / (CLOCKS_PER_SEC / 1000));

    // free block
    sdsfree(path);
    esmfree(esm);

    cleanup();
    return 0;
}
