#pragma once

#include "configuration.hpp"
#include <istream>
#include <string>
#include <unordered_map>

namespace File {

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

    std::unordered_map<std::string, Configuration> configurations;

public:
    ConfigurationManager()
    {
        configurations[defaultConfigurationName];
        configurations[gameSettingsConfigurationName];
    };
    ~ConfigurationManager() = default;

    //
    bool loadFile(const std::string& path) noexcept;

    // throws out_of_range
    const Configuration* getConfiguration(const std::string& name) { return &configurations.at(name); }
    Configuration& getMutableConfiguration(const std::string& name) { return configurations.at(name); }
    Configuration& getDefaultConfiguration() { return configurations[defaultConfigurationName]; }
    Configuration& getGameSettingsConfiguration() { return configurations[gameSettingsConfigurationName]; }

#ifdef DEBUG
    std::unordered_map<std::string, Configuration>& getAll()
    {
        return configurations;
    }
#endif

    // for \r\n compatibility
    void readLine(std::istream& input, std::string& str);
};

}; // namespace File
