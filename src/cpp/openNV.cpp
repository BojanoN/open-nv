#include "gameworld.hpp"
#include "util/logger.hpp"
#include <getopt.h>
#include <time.h>

#define FALLOUTNV_ESM_DEFAULT_PATH "./bin/esm/FalloutNV.esm"

int main(int argc, char** argv)
{

#ifdef DEBUG
    Logger::setLevel(Logger::LOG_DEBUG);
#else
    Logger::setLevel(Logger::LOG_INFO);
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

    GameWorld::GameWorld world;
    ESM::ESMReader       reader(path);
    world.load(reader);

    //log_info("Loaded %s in %f miliseconds", path.c_str(), (double)(end - start) / (CLOCKS_PER_SEC / 1000));

    return 0;
}
