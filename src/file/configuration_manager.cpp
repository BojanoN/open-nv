#include "configuration_manager.hpp"
#include "logc/log.h"
#include "types/conversion.hpp"
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>

namespace File {

using Types::ErrorPair;

namespace fs = std::filesystem;

bool ConfigurationManager::shouldSkip(const char* line, uint64_t lineLength) {
    if((lineLength == 0) || (std::strchr(line, commentStart) == line)) {
        return true;
    }
    return false;
}

bool ConfigurationManager::isCategoryName(const char* line, uint64_t lineLength) {
    if(std::strchr(line, categoryNameStart) == line && std::strrchr(line, categoryNameEnd) == (line + lineLength - 1)) {
        return true;
    }
    return false;
}

void ConfigurationManager::loadValue(const char* name, const char* value, Configuration *const currentConfiguration) {
    ErrorPair<int64_t> intValue;
    ErrorPair<uint64_t> uIntValue;
    ErrorPair<float> floatValue;

        switch (name[0]) {
            case integerPrefix:
                intValue = Types::parseInt(value);
                if(intValue.fail()){
                    log_warn("Invalid value %s for attribute %s", value, name);
                } else {
                   currentConfiguration->nSetInt(name, intValue.value);
                }

                break;
            case unsignedPrefix:
                uIntValue = Types::parseUInt(value);
                if(uIntValue.fail()){
                    log_warn("Invalid value %s for attribute %s", value, name);
                } else {
                   currentConfiguration->nSetUInt(name, uIntValue.value);
                }
                break;

            case booleanPrefix:
                intValue = Types::parseInt(value);
                if(intValue.fail()){
                    log_warn("Invalid value %s for attribute %s", value, name);
                } else {
                    currentConfiguration->nSetBool(name, intValue.value != 0 ? true : false);
                }

                break;
            case doublePrefix:
                floatValue = Types::parseFloat(value);
                if(floatValue.fail()) {
                    log_warn("Invalid value %s for attribute %s", value, name);                    
                } else {
                    currentConfiguration->nSetFloat(name, floatValue.value);                    
                }
                break;
            case altStringPrefix:
            case stringPrefix:
                currentConfiguration->nSetString(name, value);
                break;
            default:
                log_warn("Invalid type: %s, for attribute", value, name);
                break;
            
        }
}

void ConfigurationManager::setCurrentConfiguration(const char* line, uint64_t lineLength, char *const currentConfigName, Configuration* *const currentConfiguration) {
    std::memcpy(currentConfigName, &line[1], lineLength - 2); // Remove braces
    currentConfigName[lineLength - 2] = '\0';
    *currentConfiguration = &configurations[std::string(currentConfigName)];
}

Error ConfigurationManager::loadFile(const char* filename) noexcept {

    FILE* configFile = std::fopen(filename, "r");
    if(configFile == NULL) {
        log_error("Cannot open config file");
        return Err::IOError;
    }

    Configuration* currentConfiguration = &configurations[defaultConfigurationName];
    char currentConfigName[configurationNameSize];

    while (true) {

        char lineBuffer[bufferSize];

        int numberOfBytesRead = std::fscanf(configFile, lineBufferFormatString, lineBuffer);
        if(numberOfBytesRead == -1 && std::feof(configFile)) {
            return Err::Success;
        }

        if(std::ferror(configFile)) {
            log_error("Error while reading config file");
            return Err::IOError;            
        }

        uint64_t lineLength = std::strlen(lineBuffer);
        if(shouldSkip(lineBuffer, lineLength)) {
            continue;
        }

        if(isCategoryName(lineBuffer, lineLength)) {
            setCurrentConfiguration(lineBuffer, lineLength, currentConfigName, &currentConfiguration);        
            continue;
        }

        char* delimiterLoc = std::strchr(lineBuffer, optionDelimiter);
        if(delimiterLoc == NULL) {
            log_warn("Line %s does not contain an attribute", lineBuffer);
            continue;            
        }

        char* name = lineBuffer;
        *delimiterLoc = '\0';
        char* value = delimiterLoc + 1;

        loadValue(name, value, currentConfiguration);
    }
}

}; // namespace File
