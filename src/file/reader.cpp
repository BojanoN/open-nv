#include "reader.hpp"

namespace File {

//FileProvider* Reader::fileProvider = new FileProvider("/media/crop/06A0AC67A0AC5F45/GOG Games/Fallout New Vegas/Data/");
FileProvider* Reader::fileProvider = nullptr;

FileProvider& Reader::getFileProvider()
{
    return *fileProvider;
}

void Reader::setRootFilePath(const std::string& path)
{
    fileProvider = new FileProvider(path);
}

};
