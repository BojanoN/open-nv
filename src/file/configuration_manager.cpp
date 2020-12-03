#include "configuration_manager.hpp"
#include "logc/log.h"
#include <cassert>
#include <cctype>
#include <filesystem>
#include <fstream>

namespace File {

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

        try {

            switch (name[0]) {
            case integerPrefix:
                currentConfiguration->setInt(name, std::stol(value));
                break;
            case unsignedPrefix:
                currentConfiguration->setUInt(name, std::stol(value));
                break;
            case booleanPrefix:
                currentConfiguration->setBool(name, std::stoi(value) != 0 ? true : false);
                break;
            case doublePrefix:
                currentConfiguration->setDouble(name, std::stod(value));
                break;
            case altStringPrefix:
            case stringPrefix:
                currentConfiguration->setString(name, value);
                break;
            default:
                log_warn("Invalid type: %s, for attribute", value.data(), name.data());
                break;
            }

        } catch (std::invalid_argument& inv_arg) {
            log_error("Invalid value %s for attribute %s: incorrect type", value.data(), name.data());
        } catch (std::out_of_range& out_range) {
            log_error("Invalid value %s for attribute %s: value out of range", value.data(), name.data());
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
