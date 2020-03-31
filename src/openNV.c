#include <getopt.h>

#include "esm/esm.h"
#include "logc/log.h"
#include "sds/sds.h"

#define FALLOUTNV_ESM_DEFAULT_PATH "./bin/esm/FalloutNV.esm"

int main(int argc, char** argv)
{
    log_set_level(LOG_DEBUG);

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

    log_info("loading esm file from %s", path);

    Esm* esm;
    esm = esmnew(path);

    // free block
    sdsfree(path);
    esmfree(esm);
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