#include "configuration_manager.hpp"
#include "logc/log.h"
#include "types/conversion.hpp"
#include <cassert>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>

namespace File {

using Types::ErrorPair;

namespace fs = std::filesystem;

bool ConfigurationManager::loadFile(const std::string& filename) noexcept
{
    fs::path path(filename);

    if (!fs::exists(path) || !fs::is_regular_file(path)) {
        log_error("%s is not a configuration file", filename.data());
        return false;
    }

    std::ifstream configFile(path);

    if (!configFile.good()) {
        log_error("Cannot open config file");
        return false;
    }

    Configuration* currentConfiguration = &configurations[defaultConfigurationName];
    std::string    currentConfigName;

    while (true) {
        std::string line;
        readLine(configFile, line);

        if (configFile.eof()) {
            return true;
        }

        if (configFile.fail()) {
            log_error("Error while reading config file");
            return false;
        }

        if (line.empty() || line.find(commentStart) == 0) {
            continue;
        }

        if (line.find_first_of(categoryNameStart) == 0 && line.find_last_of(categoryNameEnd) == line.size() - 1) {
            currentConfigName    = line.substr(1, line.size() - 2);
            currentConfiguration = &configurations[currentConfigName];
            log_debug("Currently have %d configurations", configurations.size());
            assert(currentConfiguration == &configurations[currentConfigName]);
            continue;
        }

        int delimiterLoc = line.find_first_of(optionDelimiter);
        if (delimiterLoc == std::string::npos) {
            log_warn("Line %s does not contain an attribute", line.data());
            continue;
        }

        std::string name  = line.substr(0, delimiterLoc);
        std::string value = line.substr(delimiterLoc + 1);

        ErrorPair<int64_t> intValue;
        ErrorPair<uint64_t> uIntValue;
        ErrorPair<float> floatValue;

        switch (name[0]) {
            case integerPrefix:
                intValue = Types::parseInt(value.c_str());
                if(intValue.fail()){
                    log_warn("Invalid value %s for attribute %s", value.c_str(), name.c_str());
                } else {
                   currentConfiguration->nSetInt(name, intValue.value);
                }

                break;
            case unsignedPrefix:
                uIntValue = Types::parseUInt(value.c_str());
                if(uIntValue.fail()){
                    log_warn("Invalid value %s for attribute %s", value.c_str(), name.c_str());
                } else {
                   currentConfiguration->nSetUInt(name, uIntValue.value);
                }
                break;

            case booleanPrefix:
                intValue = Types::parseInt(value.c_str());
                if(intValue.fail()){
                    log_warn("Invalid value %s for attribute %s", value.c_str(), name.c_str());
                } else {
                    currentConfiguration->nSetBool(name, intValue.value != 0 ? true : false);
                }

                break;
            case doublePrefix:
                floatValue = Types::parseFloat(value.c_str());
                if(floatValue.fail()) {
                    log_warn("Invalid value %s for attribute %s", value.c_str(), name.c_str());                    
                } else {
                    currentConfiguration->nSetFloat(name, floatValue.value);                    
                }
                break;
            case altStringPrefix:
            case stringPrefix:
                currentConfiguration->nSetString(name, value.c_str());
                break;
            default:
                log_warn("Invalid type: %s, for attribute", value.data(), name.data());
                break;
            
        }
    }
}

void ConfigurationManager::readLine(std::istream& input, std::string& str)
{
    std::getline(input, str);
    if (str.find_last_of("\r") == str.size() - 1) {
        str.pop_back();
    }
}

}; // namespace File
