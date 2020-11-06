#include "compile.hpp"
#include "logc/log.h"
#include "parser.hpp"
#include "tokenizer.hpp"
#include "vm.hpp"
#include <dirent.h>
#include <fstream>
#include <getopt.h>
#include <unistd.h>

int main(int argc, char** argv)
{

    bool           batchRun = true;
    char           c;
    DIR*           d;
    struct dirent* dir;
    const char*    fname = argv[1];

    Script::Tokenizer* tok    = new Script::Tokenizer();
    Script::Parser*    parser = new Script::Parser();

    while ((c = getopt(argc, argv, "s:")) != -1) {
        switch (c) {
        case 's':
            batchRun = false;
            fname    = optarg;
            break;
        default:
            delete tok;
            delete parser;
            exit(3);
            break;
        }
    }

    if (batchRun) {
        std::ofstream failedCases { "failed.txt" };

        d = opendir(fname);

        if (d) {

            int passed = 0;
            int failed = 0;
            int total  = 0;

            while ((dir = readdir(d)) != NULL) {
                if (dir->d_type != DT_REG) {
                    continue;
                }

                std::string path = fname;
                path += '/';
                path += dir->d_name;

                std::ifstream in { path };

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
                        failedCases << dir->d_name << "\n";
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
            log_info("Overall success rate: %lf%%", ((double)passed / (total)) * 100);

            return 0;
        }

        return 1;
    } else {
        std::ifstream in { fname };

        in.seekg(0, std::ios::end);
        ssize_t size = in.tellg();
        in.seekg(0, std::ios::beg);

        std::string source(size, '\0');
        in.read(const_cast<char*>(source.data()), size);

        std::vector<Script::Token>* tokens = tok->getTokens(source);

        if (!tok->hadError()) {

            for (Script::Token n : *tokens) {
                std::cout << n << '\n';
            }

            try {
                std::vector<Script::Node*>* s = parser->parse(tokens);

                for (Script::Node* n : *s) {
                    delete n;
                }

                delete s;
            } catch (std::runtime_error& e) {
                log_fatal("%s", e.what());
                return 3;
            }
            delete tokens;
        }

        delete parser;
        delete tok;

        return 0;
    }
    return 2;
}
