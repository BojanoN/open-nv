#include "logc/log.h"
#include "parser.hpp"
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

    std::string source(size, '\0');
    in.read(source.data(), size);

    Script::Tokenizer* tok = new Script::Tokenizer(source);

    tok->getTokens();
    if (!tok->hadError()) {
        Script::Parser* parser = new Script::Parser(tok);

        Script::Expr* e;

        try {
            e = parser->parse();
            delete e;
        } catch (std::runtime_error& e) {
            log_fatal("%s", e.what());
        }

        delete parser;
    }
    delete tok;

    return 0;
}
