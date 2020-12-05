#include "configuration.hpp"

namespace File {


Configuration::~Configuration() {
	for(auto iterator = nValues.begin(); iterator != nValues.end(); iterator++) {
		if((iterator->first)[0] == stringPrefix) {
			if(iterator->second.szValue != NULL) {
				std::free(iterator->second.szValue);
			}
		}
	}
}

bool Configuration::contains(const std::string& name) const {

	if(nValues.find(name) == nValues.end()) {
		return false;
	} else {
		return true;
	}
}

bool Configuration::isValidConfigurationName(const std::string& name, char prefix) const {
	if(name.empty()) {
		return false;
	}

	if(name[0] != prefix) {
		return false;
	}

	return true;
}

Types::ErrorPair<uint64_t> Configuration::nGetUInt(const std::string& name) {
	if(!isValidConfigurationName(name, uIntPrefix)) {
		return Types::ErrorPair<uint64_t>(1, 0);
	}

	if(!contains(name)) {
		return Types::ErrorPair<uint64_t>(1, 0);
	}

	uint64_t configurationValue = nValues[name].uValue;
	return Types::ErrorPair<uint64_t>(0, configurationValue);
}

bool Configuration::nSetUInt(const std::string& name, uint64_t value) {
	if(!isValidConfigurationName(name, uIntPrefix)) {
		return false;
	}

	nValues[name].uValue = value;
	return true;
}

Types::ErrorPair<int64_t> Configuration::nGetInt(const std::string& name) {
	if(!isValidConfigurationName(name, intPrefix)) {
		return Types::ErrorPair<int64_t>(1, 0);
	}

	if(!contains(name)) {
		return Types::ErrorPair<int64_t>(1, 0);
	}

	return Types::ErrorPair<int64_t>(0, nValues[name].iValue);
}

bool Configuration::nSetInt(const std::string& name, int64_t value) {
	if(!isValidConfigurationName(name, intPrefix)) {
		return false;
	}

	nValues[name].iValue = value;
	return true;
}

Types::ErrorPair<float> Configuration::nGetFloat(const std::string& name) {
	if(!isValidConfigurationName(name, floatPrefix)) {
		return Types::ErrorPair<float>(1, 0);
	}

	if(!contains(name)) {
		return Types::ErrorPair<float>(1, 0);
	}

	return Types::ErrorPair<float>(0, nValues[name].fValue);
}

bool Configuration::nSetFloat(const std::string& name, float value) {
	if(!isValidConfigurationName(name, floatPrefix)) {
		return false;
	}

	nValues[name].fValue = value;
	return true;
}

Types::ErrorPair<bool> Configuration::nGetBool(const std::string& name) {
	if(!isValidConfigurationName(name, boolPrefix)) {
		return Types::ErrorPair<bool>(1, 0);
	}

	if(!contains(name)) {
		return Types::ErrorPair<bool>(1, 0);
	}

	return Types::ErrorPair<bool>(0, nValues[name].bValue);
}

bool Configuration::nSetBool(const std::string& name, bool value) {
	if(!isValidConfigurationName(name, boolPrefix)) {
		return false;
	}

	nValues[name].bValue = value;
	return true;
}

Types::ErrorPair<const char*> Configuration::nGetString(const std::string& name) {
	if(!isValidConfigurationName(name, stringPrefix)) {
		return Types::ErrorPair<const char*>(1, 0);
	}

	if(!contains(name)) {
		return Types::ErrorPair<const char*>(1, 0);
	}

	return Types::ErrorPair<const char*>(0, (const char*) nValues[name].szValue);
}

bool Configuration::nSetString(const std::string& name, const char* value) {
	if(!isValidConfigurationName(name, stringPrefix)) {
		return false;
	}

	char* configurationValue = nValues[name].szValue;
	std::size_t memorySize = std::strlen(value) + 1;

	if(configurationValue == NULL) {
		
		char* newMemory = (char*) std::malloc(sizeof(char) * memorySize);
		if(newMemory == NULL) {
			return false;
		}
		configurationValue = newMemory;

	} else if(std::strlen(configurationValue) + 1 != memorySize) {
		
		char* newMemory = (char*) std::realloc(configurationValue, sizeof(char) * memorySize);
		if(newMemory == NULL) {
			return false;
		}
		
		configurationValue = newMemory;
	}

	std::strcpy(configurationValue, value);
	nValues[name].szValue = configurationValue;
	return true;
}

};
