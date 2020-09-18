#include "reader.hpp"

namespace File {

FileProvider* Reader::fileProvider = new FileProvider("/home/bojan/Documents/projects/open-nv/bin/bsa");

FileProvider& Reader::getFileProvider()
{
    return *fileProvider;
}

void Reader::setRootFilePath(const std::string& path)
{
    fileProvider = new FileProvider(path);
}

};
