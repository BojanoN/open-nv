#include "gameworld.hpp"
#include "logc/log.h"
#include <getopt.h>
#include <time.h>
#include "file/reader.hpp"
#include <chrono>

#define FALLOUTNV_ESM_DEFAULT_PATH "./bin/esm/FalloutNV.esm"

int main(int argc, char** argv)
{

#ifdef DEBUG
    log_set_level(LOG_DEBUG);
#else
    log_set_level(LOG_INFO);
#endif
    //log_set_level(LOG_INFO);
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

    File::Reader::setRootFilePath(path.c_str());
    std::string esmPath = std::string(path) + std::string("FalloutNV.esm");

    auto start = std::chrono::system_clock::now();

    GameWorld::GameWorld world;
    try {
        ESM::ESMReader reader(esmPath);
        world.load(reader);
    } catch (std::runtime_error& e) {
        log_fatal(e.what());
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << diff.count() << std::endl;

    GameWorld::GameData<ESM::Script>* scriptStore = (GameWorld::GameData<ESM::Script>*)world.getDataStore(ESM::ESMType::SCPT);

    std::unordered_map<formid, ESM::Script*>& scriptMap = scriptStore->getMap();

    for (auto& n : scriptMap) {

        std::string   filename = std::to_string(n.first) + ".bin";
        std::ofstream out { "./dump/" + filename };

        std::vector<uint8_t>& source = n.second->data.compiledSource;

        out.write((char*)source.data(), source.size());
        out.close();
    }

    File::Reader::destroyFileProvider();
    //log_info("Loaded %s in %f miliseconds", path.c_str(), (double)(end - start) / (CLOCKS_PER_SEC / 1000));

    return 0;
}
