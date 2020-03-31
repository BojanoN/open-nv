#include "logc/log.h"

int main(int argc, char** argv)
{
    log_set_level(LOG_DEBUG);
    log_info("Hello mom");
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