#include "gameworld.hpp"
#include "logc/log.h"
#include <getopt.h>
#include <string.h>
#include <time.h>
#define FALLOUTNV_ESM_DEFAULT_PATH "./bin/esm/FalloutNV.esm"

int main(int argc, char** argv)
{

#ifdef DEBUG
    log_set_level(LOG_DEBUG);
#else
    log_set_level(LOG_INFO);
#endif
    //    log_set_level(LOG_INFO);
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

    GameWorld::GameWorld* world = new GameWorld::GameWorld();
    try {
        ESM::ESMReader reader(path);
        world->load(reader);
    } catch (std::runtime_error& e) {
        log_fatal(e.what());
        delete world;
    }

    GameWorld::GameData<ESM::GameSetting>* sStore = (GameWorld::GameData<ESM::GameSetting>*)world->getDataStore(ESM::ESMType::GMST);
    ;
    std::string test = "sSleepDeprivationIncrease";
    assert(sStore->get() != nullptr);
    /*
    GameWorld::GameData<ESM::Script>* scriptStore = (GameWorld::GameData<ESM::Script>*)world.getDataStore(ESM::ESMType::SCPT);

    std::unordered_map<formid, ESM::Script*>& scriptMap = scriptStore->getMap();

    for (auto& n : scriptMap) {

        std::string   filename = std::to_string(n.first) + ".bin";
        std::ofstream out { "./dump/" + filename };

        std::vector<uint8_t>& source = n.second->data.compiledSource;

        out.write((char*)source.data(), source.size());
        out.close();
    }
    */
    //log_info("Loaded %s in %f miliseconds", path.c_str(), (double)(end - start) / (CLOCKS_PER_SEC / 1000));
    delete world;
    return 0;
}
