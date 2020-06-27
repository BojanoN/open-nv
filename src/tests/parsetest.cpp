#include "compile.hpp"
#include "interpreter.hpp"
#include "logc/log.h"
#include "parser.hpp"
#include "tokenizer.hpp"
#include "vm.hpp"
#include <dirent.h>
#include <fstream>
#include <unistd.h>

int main(int argc, char** argv)
{

    if (argc != 2) {
        log_fatal("%s", "Only one folder please!");
        return 1;
    }

    DIR*           d;
    struct dirent* dir;
    d = opendir(argv[1]);
    chdir(argv[1]);

    Script::Tokenizer* tok    = new Script::Tokenizer();
    Script::Parser*    parser = new Script::Parser();

    if (d) {

        int passed = 0;
        int failed = 0;
        int total  = 0;

        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type != DT_REG) {
                continue;
            }

            printf("PENIS: %s\n", dir->d_name);

            std::ifstream in { dir->d_name };

            if (in.fail()) {
                log_error("Failed to open file %s!", dir->d_name);
                continue;
            }

            total++;
            in.seekg(0, std::ios::end);
            ssize_t size = in.tellg();
            in.seekg(0, std::ios::beg);

            std::string source(size, '\0');
            in.read(const_cast<char*>(source.data()), size);

            std::vector<Script::Token>* tokens = tok->getTokens(source);

            if (!tok->hadError()) {

                try {
                    std::vector<Script::Node*>* s = parser->parse(tokens);

                    for (Script::Node* n : *s) {
                        delete n;
                    }

                    delete s;
                    passed++;
                } catch (std::exception& e) {
                    log_fatal("%s\n", e.what());
                    failed++;
                }

                delete tokens;
            }
            log_info("Scripts successfully parsed: %d", passed);
            log_info("Scripts unsuccessfully parsed: %d", failed);
        }

        delete parser;
        delete tok;
        closedir(d);

        log_info("Scripts successfully parsed: %d", passed);
        log_info("Scripts unsuccessfully parsed: %d", failed);
        log_info("Overall success rate: %lf", (double)passed / (total));

        return 0;
    }

    return 1;
}
