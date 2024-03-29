#include "compile.hpp"
#include "interpreter.hpp"
#include "logc/log.h"
#include "parser.hpp"
#include "tokenizer.hpp"
#include "vm.hpp"
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
    in.read(const_cast<char*>(source.data()), size);

    Script::Tokenizer* tok = new Script::Tokenizer();

    std::vector<Script::Token>* tokens = tok->getTokens(source);

    if (!tok->hadError()) {
        Script::Parser* parser = new Script::Parser();

        try {
            Script::VM*                 vm = new Script::VM();
            std::vector<Script::Node*>* s  = parser->parse(tokens);

            for (Script::Node* n : *s) {
                log_info("Node: %s", Script::NodeEnumToString(n->type));
            }

            for (Script::Node* n : *s) {
                n->print();
            }

            Script::Compiler*       c  = new Script::Compiler(s);
            Script::CompiledScript* cs = c->compile();

            if (cs != nullptr) {

                cs->print();

                vm->execute(cs);
                delete cs;
            }

            for (Script::Node* n : *s) {
                delete n;
            }

            delete vm;
            delete c;
            delete s;
        } catch (std::runtime_error& e) {
            log_fatal("%s", e.what());
        }

        delete tokens;
        delete parser;
    }
    delete tok;

    return 0;
}
