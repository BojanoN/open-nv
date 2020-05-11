#include "bsa/bsa.hpp"
#include "bsa/hash.hpp"
#include "logc/log.h"

int main(void)
{

    std::string updateBSA  = "./bin/bsa/Update.bsa";
    std::string textureBSA = "./bin/bsa/Fallout - Textures.bsa";

    BSA::BSA* bsa;

    try {
        bsa = new BSA::BSA(updateBSA);
    } catch (std::exception& e) {
        log_fatal("Error while parsing BSA file!");
        log_fatal(e.what());
        return 1;
    }

    // Uncompressed file test
    // Should succeed
    std::vector<uint8_t>* ret = bsa->getFile("meshes\\architecture\\mccarran\\mcmarranwallsdes\\walllong.nif");
    // Should fail
    bsa->getFile("meshes\\architecture\\mccaan\\mcmarranwallsdes\\walllong.nif");
    delete ret;
    // Should be served from cache
    ret = bsa->getFile("meshes\\architecture\\mccarran\\mcmarranwallsdes\\walllong.nif");
    delete ret;

    std::string correctPath = "meshes\\architecture\\mccarran\\mcmarranwallsdes\\walllong.nif";
    std::string wrongPath   = "meshes\\architecture\\mccaan\\mcmades\\wallg.nif";

    bool exists = bsa->fileExists(correctPath);
    log_debug("File %s exists in %s: %s", correctPath.c_str(), updateBSA.c_str(), exists ? "true" : "false");

    exists = bsa->fileExists(wrongPath);
    log_debug("File %s exists in %s: %s", wrongPath.c_str(), updateBSA.c_str(), exists ? "true" : "false");

    delete bsa;

    // Compressed file test
    try {
        bsa = new BSA::BSA(textureBSA);
    } catch (std::exception& e) {
        log_fatal("Error while parsing BSA file!");
        log_fatal(e.what());
        return 1;
    }

    ret = bsa->getFile("textures\\clutter\\snowglobes\\snowglobenelisafb_d.dds");
    delete ret;

    delete bsa;

    return 0;
}
