#pragma once
//#include "logc/log.h"
#include "types/errorpair.hpp"
#include "error/error.hpp"

#include <filesystem>
#include <unordered_map>
#include <string>
#include <variant>
#include <cstring>
#include <cstdint>


namespace fs = std::filesystem;

namespace File {

using Err::Error;
// TODO: refactor strings to remove copying and allocation

class Configuration {

	union ConfigurationValue {

		uint64_t uValue;
		int64_t iValue;
		float fValue;
		bool bValue;
		char* szValue;

	};

private:

	static inline const char uIntPrefix = 'u'; 
	static inline const char intPrefix = 'i';
	static inline const char floatPrefix = 'f';
	static inline const char boolPrefix = 'b';
	static inline const char stringPrefix = 's'; 

	std::unordered_map<std::string, ConfigurationValue> nValues;

	std::unordered_map<std::string, std::variant<std::string, int64_t, uint64_t, double, bool>> values;

	//throws runtime_error if nonexistent or wrong type
	template <typename T>
	const T& get(const std::string& name) const;

	template <typename T>
	void set(const std::string& name, T value); 

	Error isValidConfigurationName(const std::string& name, char prefix) const;

public:

	Configuration() = default;
	~Configuration();

	const std::string& getString(const std::string& name) const { return get<std::string>(name); }
	const int64_t& getInt(const std::string& name) const { return get<int64_t>(name); }
	const uint64_t& getUInt(const std::string& name) const { return get<uint64_t>(name); }
	const double& getDouble(const std::string& name) const { return get<double>(name); }
	const bool& getBool(const std::string& name) const { return get<bool>(name); }

	Types::ErrorPair<uint64_t> nGetUInt(const std::string& name);
	Types::ErrorPair<int64_t> nGetInt(const std::string& name);
	Types::ErrorPair<float> nGetFloat(const std::string& name);
	Types::ErrorPair<bool> nGetBool(const std::string& name);
	Types::ErrorPair<const char*> nGetString(const std::string& name);
	
	bool contains(const std::string& name) const;

	Error nSetString(const std::string& name, const char* value);
	Error nSetInt(const std::string& name, int64_t value);
	Error nSetUInt(const std::string& name, uint64_t value);
	Error nSetFloat(const std::string& name, float value);
	Error nSetBool(const std::string& name, bool value);


	void setString(const std::string& name, std::string value) { set<std::string>(name, value);}
	void setInt(const std::string& name, int64_t value) { set<int64_t>(name, value);}
	void setUInt(const std::string& name, uint64_t value) { set<uint64_t>(name, value);}
	void setDouble(const std::string& name, double value) { set<double>(name, value);}
	void setBool(const std::string& name, bool value) { set<bool>(name, value);}

#ifdef DEBUG
	std::unordered_map<std::string, std::variant<std::string, int64_t, uint64_t, double, bool>>& getAll() {return values;}
#endif

};




template <typename T>
const T& Configuration::get(const std::string& name) const {
	
	try {
		return std::get<T>(values.at(name));
	
	} catch(std::bad_variant_access& bad_access) {
		throw std::runtime_error(bad_access.what());
	
	} catch(std::out_of_range& out_range) {
		throw std::runtime_error(out_range.what());
	}
}

template <typename T>
void Configuration::set(const std::string& name, T value) {
	values[name] = value;
}

}; // namespace File