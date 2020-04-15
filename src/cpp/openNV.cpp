#include "esm/esm.hpp"
#include "logc/log.h"
#include <getopt.h>
#include <time.h>

#define FALLOUTNV_ESM_DEFAULT_PATH "./bin/esm/FalloutNV.esm"

int main(int argc, char** argv)
{

#ifdef DEBUG
    log_set_level(LOG_DEBUG);
#else
    log_set_level(LOG_INFO);
#endif

    int         c;
    std::string path = FALLOUTNV_ESM_DEFAULT_PATH;

    while ((c = getopt(argc, argv, "f:")) != -1) {
        switch (c) {
        case 'f':
            path = optarg;
            break;
        default:
            break;
        }
    }

    ESM::ESM* esm;
    clock_t   start = clock();
    esm             = new ESM::ESM(path);
    clock_t end     = clock();

    log_info("Loaded %s in %f miliseconds", path.c_str(), (double)(end - start) / (CLOCKS_PER_SEC / 1000));

    delete esm;
    return 0;
}