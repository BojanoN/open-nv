#include "logc/log.h"
#include "tokenizer.hpp"
#include <fstream>

int main(int argc, char** argv)
{

    if (argc != 2) {
        log_fatal("%s", "Only one file please!");
        return 1;
    }

    std::ifstream in { argv[1] };

    in.seekg(0, std::ios::end);
    ssize_t size = in.tellg();
    in.seekg(0, std::ios::beg);

    std::string source(size + 1, '\0');
    in.read(source.data(), size);

    Script::Tokenizer* tok = new Script::Tokenizer(source);

    tok->getTokens();
    tok->printTokens();

    delete tok;

    return 0;
}
