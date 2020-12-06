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

Error Configuration::isValidConfigurationName(const std::string& name, char prefix) const {
	if(name.empty()) {
		return Err::InvalidArgument;
	}

	if(name[0] != prefix) {
		return Err::TypeError;
	}

	return Err::Success;
}

Types::ErrorPair<uint64_t> Configuration::nGetUInt(const std::string& name) {
	Error nameError = isValidConfigurationName(name, uIntPrefix);
	if(nameError.fail()) {
		return Types::ErrorPair<uint64_t>(nameError);
	}

	if(!contains(name)) {
		return Types::ErrorPair<uint64_t>(Err::KeyNotFound);
	}

	uint64_t configurationValue = nValues[name].uValue;
	return Types::ErrorPair<uint64_t>(Err::Success, configurationValue);
}

Error Configuration::nSetUInt(const std::string& name, uint64_t value) {
	Error nameError = isValidConfigurationName(name, uIntPrefix);
	if(nameError.fail()) {
		return nameError;
	}

	nValues[name].uValue = value;
	return Err::Success;
}

Types::ErrorPair<int64_t> Configuration::nGetInt(const std::string& name) {
	Error nameError = isValidConfigurationName(name, intPrefix);
	if(nameError.fail()) {
		return Types::ErrorPair<int64_t>(nameError);
	}

	if(!contains(name)) {
		return Types::ErrorPair<int64_t>(Err::KeyNotFound);
	}

	return Types::ErrorPair<int64_t>(Err::Success, nValues[name].iValue);
}

Error Configuration::nSetInt(const std::string& name, int64_t value) {
	Error nameError = isValidConfigurationName(name, intPrefix);
	if(nameError.fail()) {
		return nameError;
	}

	nValues[name].iValue = value;
	return Err::Success;
}

Types::ErrorPair<float> Configuration::nGetFloat(const std::string& name) {
	Error nameError = isValidConfigurationName(name, floatPrefix);
	if(nameError.fail()) {
		return Types::ErrorPair<float>(nameError);
	}

	if(!contains(name)) {
		return Types::ErrorPair<float>(Err::KeyNotFound);
	}

	return Types::ErrorPair<float>(Err::Success, nValues[name].fValue);
}

Error Configuration::nSetFloat(const std::string& name, float value) {
	Error nameError = isValidConfigurationName(name, floatPrefix);
	if(nameError.fail()) {
		return nameError;
	}

	nValues[name].fValue = value;
	return Err::Success;
}

Types::ErrorPair<bool> Configuration::nGetBool(const std::string& name) {
	Error nameError = isValidConfigurationName(name, boolPrefix);
	if(nameError.fail()) {
		return Types::ErrorPair<bool>(nameError);
	}

	if(!contains(name)) {
		return Types::ErrorPair<bool>(Err::KeyNotFound);
	}

	return Types::ErrorPair<bool>(Err::Success, nValues[name].bValue);
}

Error Configuration::nSetBool(const std::string& name, bool value) {
	Error nameError = isValidConfigurationName(name, boolPrefix);
	if(nameError.fail()) {
		return nameError;
	}

	nValues[name].bValue = value;
	return Err::Success;
}

Types::ErrorPair<const char*> Configuration::nGetString(const std::string& name) {
	Error nameError = isValidConfigurationName(name, stringPrefix);
	if(nameError.fail()) {
		return Types::ErrorPair<const char*>(nameError);
	}

	if(!contains(name)) {
		return Types::ErrorPair<const char*>(Err::KeyNotFound);
	}

	return Types::ErrorPair<const char*>(Err::Success, (const char*) nValues[name].szValue);
}

Error Configuration::nSetString(const std::string& name, const char* value) {
	Error nameError = isValidConfigurationName(name, stringPrefix);
	if(nameError.fail()) {
		return nameError;
	}

	char* configurationValue = nValues[name].szValue;
	std::size_t memorySize = std::strlen(value) + 1;

	if(configurationValue == NULL) {
		
		char* newMemory = (char*) std::malloc(sizeof(char) * memorySize);
		if(newMemory == NULL) {
			return Err::MemoryError;
		}
		configurationValue = newMemory;

	} else if(std::strlen(configurationValue) + 1 != memorySize) {
		
		char* newMemory = (char*) std::realloc(configurationValue, sizeof(char) * memorySize);
		if(newMemory == NULL) {
			return Err::MemoryError;
		}
		
		configurationValue = newMemory;
	}

	std::strcpy(configurationValue, value);
	nValues[name].szValue = configurationValue;
	return Err::Success;
}

};
