#pragma once

#include "configuration.hpp"
#include "error/error.hpp"
#include "types/errorpair.hpp"
#include <istream>
#include <string>
#include <unordered_map>

namespace File {

using Err::Error;
using Types::ErrorPair;

class ConfigurationManager {

private:
    static const char               commentStart                  = ';';
    static const char               categoryNameStart             = '[';
    static const char               categoryNameEnd               = ']';
    inline static const std::string defaultConfigurationName      = "_default_";
    inline static const std::string gameSettingsConfigurationName = "GameSettings";

    static const char optionDelimiter = '=';
    static const char integerPrefix   = 'i';
    static const char booleanPrefix   = 'b';
    static const char doublePrefix    = 'f';
    static const char stringPrefix    = 's';
    static const char altStringPrefix = 'S';

    static const char unsignedPrefix = 'u';

    static const uint32_t bufferSize = 4096;
    inline static const char* lineBufferFormatString = " %4096[^\r\n]";
    static const uint32_t configurationNameSize = 128;

    std::unordered_map<std::string, Configuration> configurations;

    bool shouldSkip(const char* line, uint64_t lineLength);
    bool isCategoryName(const char* line, uint64_t lineLength);
    void loadValue(const char* name, const char* value, Configuration *const currentConfiguration);
    void setCurrentConfiguration(const char* line, uint64_t lineLength, char *const currentConfigName, Configuration* *const currentConfiguration);

public:
    ConfigurationManager()
    {
        configurations[defaultConfigurationName];
        configurations[gameSettingsConfigurationName];
    };
    ~ConfigurationManager() = default;

    //
    Error loadFile(const char* path) noexcept;

    // throws out_of_range
    Configuration& getConfiguration(const std::string& name) { return configurations.at(name); }
    Configuration& getDefaultConfiguration() { return configurations[defaultConfigurationName]; }
    Configuration& getGameSettingsConfiguration() { return configurations[gameSettingsConfigurationName]; }

    ErrorPair<Configuration*> nGetConfiguration(const char* configurationName) {
        std::string sName(configurationName);
        auto iterator = configurations.find(sName);
        if(iterator == configurations.end()) {
            return ErrorPair<Configuration*>(Err::InvalidArgument);
        }
        return ErrorPair<Configuration*>(Err::Success, &iterator->second);
    }

#ifdef DEBUG
    std::unordered_map<std::string, Configuration>& getAll()
    {
        return configurations;
    }
#endif
};

}; // namespace File
