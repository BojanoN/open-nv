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

    Script::Tokenizer* tok = new Script::Tokenizer(source);

    tok->getTokens();
    tok->printTokens();

    if (!tok->hadError()) {
        Script::Parser* parser = new Script::Parser(tok);

        try {
            Script::VM*                 vm = new Script::VM();
            std::vector<Script::Node*>* s  = parser->parse();

            for (Script::Node* n : *s) {
                log_info("Node: %s", Script::NodeEnumToString(n->type));
            }

            for (Script::Node* n : *s) {
                n->print();
            }

            Script::Compiler*       c  = new Script::Compiler(s);
            Script::CompiledScript* cs = c->compile();

            cs->print();

            vm->execute(cs);

            for (Script::Node* n : *s) {
                delete n;
            }

            delete vm;
            delete c;
            delete cs;
            delete s;
        } catch (std::runtime_error& e) {
            log_fatal("%s", e.what());
        }

        delete parser;
    }
    delete tok;

    return 0;
}
