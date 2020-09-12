#include "reader.hpp"

namespace File {

std::unique_ptr<FileProvider> Reader::fileProvider = std::unique_ptr<FileProvider>(new FileProvider("/home/bojan/Documents/projects/open-nv/bin/bsa"));

FileProvider& Reader::getFileProvider()
{
    return *fileProvider;
}

void Reader::setRootFilePath(const std::string& path)
{
    fileProvider = std::make_unique<FileProvider>(path);
}

};
