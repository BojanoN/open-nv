#include "reader.hpp"

namespace File {

std::unique_ptr<FileProvider> Reader::fileProvider = std::unique_ptr<FileProvider>(nullptr);

FileProvider& Reader::getFileProvider() {
	return *fileProvider;
}

void Reader::setRootFilePath(const std::string& path) {
	fileProvider = std::make_unique<FileProvider>(path);
}

};