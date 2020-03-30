#include "parse/esm.hpp"
#include "spdlog/spdlog.h"
#include <unistd.h>

const char* opts = "f:";
const char* default_path = "./bin/esm/FalloutNV.esm";

int main(int argc, char** argv)
{
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
