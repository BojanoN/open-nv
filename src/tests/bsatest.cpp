#include "bsa/bsa.hpp"
#include "bsa/hash.hpp"
#include "logc/log.h"

#define BSA_PATH "./bin/bsa/Update.bsa"

int main(void)
{

    BSA::BSA* bsa;

    try {
        bsa = new BSA::BSA(BSA_PATH);
    } catch (std::exception& e) {
        log_fatal("Error while parsing BSA file!");
        log_fatal(e.what());
        return 1;
    }

    std::vector<uint8_t>* ret = bsa->getFile("meshes\\architecture\\mccarran\\mcmarranwallsdes\\walllong.nif");

    delete ret;
    delete bsa;
    return 0;
}
