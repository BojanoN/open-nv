#include "stringutils.hpp"

namespace StringUtils {

bool endsWith(const std::string& target, const std::string& pattern) {
	return target.rfind(pattern) == target.size() - pattern.size();
}

bool startsWith(const std::string& target, const std::string& pattern) {
	return target.find(pattern) == 0;
}

std::string replaceChar(const std::string& target, char oldVal, char newVal) {
	
	std::string replaced = target;

	while(true) {
		std::size_t loc = replaced.find(oldVal);
		if(loc == std::string::npos) {
			break;
		}

		replaced[loc] = newVal;
	}

	return replaced;
}

};