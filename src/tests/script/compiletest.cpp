#include "compile.hpp"
#include "../gameworld.hpp"
#include "logc/log.h"
#include "parser.hpp"
#include "tokenizer.hpp"
#include <dirent.h>
#include <fstream>
#include <getopt.h>
#include <sys/stat.h>
#include <unistd.h>

bool compareCompiled(Script::CompiledScript* out, std::vector<uint8_t> original)
{

    if (out->getSize() != original.size())
        return false;

    for (uint32_t i = 0; i < original.size(); i++) {
        if (out->readAt(i) != original[i]) {
            return false;
        }
    }

    return true;
}

int main(int argc, char** argv)
{

    bool           batchRun = true;
    char           c;
    DIR*           d;
    struct dirent* dir;

    const char* fname = argv[1];

    std::string path = "../../../bin/esm/FalloutNV.esm";

    GameWorld::GameWorld world;
    try {
        ESM::ESMReader reader(path);
        world.load(reader);
    } catch (std::runtime_error& e) {
        log_fatal(e.what());
        return 1;
    }

    Script::Tokenizer* tok      = new Script::Tokenizer();
    Script::Parser*    parser   = new Script::Parser();
    Script::Compiler*  compiler = new Script::Compiler(&world);

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

        struct stat path_stat;
        stat(fname, &path_stat);

        if (S_ISREG(path_stat.st_mode)) {
            log_fatal("%s is not a directory", fname);
            exit(3);
        }

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
                std::string formid(dir->d_name, strchr(dir->d_name, '.') - dir->d_name);

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
                        std::vector<uint8_t>        original;
                        std::vector<Script::Node*>* s = parser->parse(tokens);

                        Script::CompiledScript* out = compiler->compile(s);

                        if (out == nullptr) {
                            failedCases << dir->d_name << "\n";
                            failed++;
                        } else {

                            std::string origName = "./compiled";
                            std::string split(strchr(formid.c_str(), '/'));

                            origName += split;
                            origName += ".bin";
                            log_info("ASDF: %s", origName.c_str());
                            std::ifstream originalFile { origName };

                            if (originalFile.fail()) {
                                return 1;
                            }

                            originalFile.seekg(0, std::ios::end);
                            ssize_t origSize = originalFile.tellg();
                            originalFile.seekg(0, std::ios::beg);
                            original.resize(origSize);
                            originalFile.read(reinterpret_cast<char*>(&original[0]), origSize);

                            if (compareCompiled(out, original)) {
                                passed++;
                            } else {
                                failedCases << dir->d_name << "\n";
                                failed++;
                            }
                            delete out;
                        }
                        for (Script::Node* n : *s) {
                            delete n;
                        }

                        delete s;
                    } catch (std::runtime_error& e) {
                        log_fatal("%s\n", e.what());
                        failedCases << dir->d_name << "\n";
                        failed++;
                    }

                    delete tokens;
                }
                log_info("Scripts successfully compiled: %d", passed);
                log_info("Scripts unsuccessfully compiled: %d", failed);
            }

            delete parser;
            delete tok;
            delete compiler;
            closedir(d);

            log_info("Scripts successfully compiled: %d", passed);
            log_info("Scripts unsuccessfully compiled: %d", failed);
            log_info("Overall success rate: %lf%%", ((double)passed / (total)) * 100);

            return 0;
        }

        return 1;
    } else {

        std::ifstream in { fname };

        struct stat path_stat;
        stat(fname, &path_stat);

        if (!S_ISREG(path_stat.st_mode)) {
            log_fatal("%s is a directory", fname);
            exit(3);
        }

        in.seekg(0, std::ios::end);
        ssize_t size = in.tellg();
        in.seekg(0, std::ios::beg);

        std::string formid(fname, strchr(fname, '.') - fname);
        std::cout << formid;
        std::string source(size, '\0');
        in.read(const_cast<char*>(source.data()), size);

        std::vector<Script::Token>* tokens = tok->getTokens(source);

        if (!tok->hadError()) {

            try {
                std::vector<uint8_t>        original;
                std::vector<Script::Node*>* s = parser->parse(tokens);

                for (Script::Node* n : *s) {
                    n->print();
                }

                Script::CompiledScript* out = compiler->compile(s);

                if (out == nullptr) {
                    log_fatal("Error while compiling the script");
                } else {
                    std::string origName = "./compiled";
                    std::string split(strchr(formid.c_str(), '/'));

                    origName += split;
                    origName += ".bin";
                    log_info("ASDF: %s", origName.c_str());
                    std::ifstream originalFile { origName };

                    if (originalFile.fail()) {
                        return 1;
                    }

                    originalFile.seekg(0, std::ios::end);
                    ssize_t origSize = originalFile.tellg();
                    originalFile.seekg(0, std::ios::beg);
                    original.resize(origSize);
                    originalFile.read(reinterpret_cast<char*>(&original[0]), origSize);

                    if (!compareCompiled(out, original)) {
                        log_fatal("Compiled script differs from original");
                        out->dumpToFile("./failed.bin");
                    }

                    delete out;
                }
                for (Script::Node* n : *s) {
                    delete n;
                }

                delete s;
            } catch (std::exception& e) {
                log_fatal("%s\n", e.what());
            }

            delete tokens;
        }

        delete parser;
        delete tok;
        delete compiler;

        return 0;
    }
    return 2;
}
