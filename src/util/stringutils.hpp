#pragma once
#include <string>
#include <cstddef>

namespace StringUtils {

bool endsWith(const std::string& target, const std::string& pattern);

bool startsWith(const std::string& target, const std::string& pattern);
  
std::string replaceChar(const std::string& target, char oldVal, char newVal);

};