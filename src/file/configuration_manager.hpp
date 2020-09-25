#pragma once

#include "configuration.hpp"
#include <unordered_map>
#include <string>
#include <istream>


namespace File {

class ConfigurationManager {

private:

	static const char commentStart = ';';
	static const char categoryNameStart = '[';
	static const char categoryNameEnd = ']';
	inline static const std::string defaultConfigurationName = "_default_";

	static const char optionDelimiter = '=';
	static const char integerPrefix = 'i';
	static const char booleanPrefix = 'b';
	static const char doublePrefix =  'f';
	static const char stringPrefix =  's';
	static const char unsignedPrefix = 'u';

	std::unordered_map<std::string, Configuration> configurations;

public:

	ConfigurationManager() { configurations[defaultConfigurationName]; };
	~ConfigurationManager() = default;

	// throws invalid_argument if file cannot be opened
	// throws runtime_error if error while reading
	void loadFile(const std::string& path);

	// throws out_of_range
	Configuration& getConfiguration(const std::string& name) { return configurations.at(name); }
	Configuration& getDefaultConfiguration() { return configurations[defaultConfigurationName]; }

#ifdef DEBUG
	std::unordered_map<std::string, Configuration>& getAll() {return configurations;}
#endif

	// for \r\n compatibility
	void readLine(std::istream& input, std::string& str);
};

}; // namespace File