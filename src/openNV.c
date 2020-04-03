#include <getopt.h>

#include "esm/esm.h"
#include "logc/log.h"
#include "sds/sds.h"

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

void init(void){
  INIT_CONSTRUCTOR_MAP(Record);
  INIT_DESTRUCTOR_MAP(Record);

  INIT_CONSTRUCTOR_MAP(Subrecord);
  INIT_DESTRUCTOR_MAP(Subrecord);
}

void cleanup(void){
  FREE_CONSTRUCTOR_MAP(Record);
  FREE_DESTRUCTOR_MAP(Record);

  FREE_CONSTRUCTOR_MAP(Subrecord);
  FREE_DESTRUCTOR_MAP(Subrecord);
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

    Esm* esm;
    esm = esmnew(path);

    // free block
    sdsfree(path);
    esmfree(esm);
    
    cleanup();
    return 0;
}
#if 0
#include "parse/esm.hpp"
#include "spdlog/spdlog.h"
#include <unistd.h>

const char* opts = "f:";
const char* default_path = "./bin/esm/FalloutNV.esm";

int main(int argc, char** argv)
{
    spdlog::set_level(spdlog::level::debug);

    int c;
    const char* path = default_path;
    ESM* esm_file;

    while ((c = getopt(argc, argv, opts)) != -1) {
        switch (c) {
        case 'f':
            path = optarg;
            break;
        default:
            break;
        }
    }

    spdlog::info("Loading main .esm file at path {}", path);
    esm_file = new ESM(path);

    delete esm_file;
    return 0;
}
#endif
