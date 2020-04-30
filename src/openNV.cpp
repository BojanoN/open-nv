#include "gameworld.hpp"
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
    log_set_level(LOG_INFO);
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

    GameWorld::GameWorld world;
    try {
        ESM::ESMReader reader(path);
        world.load(reader);
    } catch (std::runtime_error& e) {
        log_fatal(e.what());
    }
    //log_info("Loaded %s in %f miliseconds", path.c_str(), (double)(end - start) / (CLOCKS_PER_SEC / 1000));

    return 0;
}
