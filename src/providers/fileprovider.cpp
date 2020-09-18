#include "fileprovider.hpp"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <thread>

namespace fs = std::filesystem;

bool FileProvider::instantiated = false;

FileProvider::FileProvider(const std::string& rootPath)
{
    assert(!this->instantiated);

    fs::path root(rootPath);

    if (!fs::exists(root)) {
        throw std::invalid_argument(std::string("Root data path does not exist! ") + rootPath);
    }

    if (!fs::is_directory(root)) {
        throw std::invalid_argument(std::string("Root data path is not a directory! ") + rootPath);
    }

    this->root = root;
    log_info("Setting root data folder: %s", rootPath.c_str());

    this->initArchives();

    this->instantiated = true;
}

FileProvider::~FileProvider()
{
    this->instantiated = false;
    for(unsigned int i = 0, limit = archives.size(); i < limit; i++) {
        delete archives[i];
    }
}

void FileProvider::initArchives()
{

    for (auto& entry : fs::directory_iterator(this->root)) { // fs::directory_entry
        std::string name = entry.path().filename().string();
        if (StringUtils::endsWith(name, ".bsa")) {

            try {
                log_info("Loading %s", entry.path().string().c_str());
                std::string path = entry.path().string();
                this->archives.push_back(new BSA::BSA(path, path));
            } catch (std::runtime_error& e) {
                log_error(e.what());
            }
        }
    }
}

std::vector<uint8_t> FileProvider::readFile(const std::string& filepath)
{
#ifdef __linux__
    fs::path fullPath = this->root / StringUtils::replaceChar(filepath, '\\', '/');
#else
    fs::path fullPath = this->root / filepath;
#endif

    log_info("Searching for loose file: %s", fullPath.string().c_str());
    if(fs::exists(fullPath) && fs::is_regular_file(fullPath)) {
        log_debug("Opening loose file: %s", fullPath.c_str());
        return readLooseFile(fullPath);
    } 
    
    return readArchiveFile(filepath);

}

std::vector<uint8_t> FileProvider::readLooseFile(const std::string& filepath)
{

    FILE* file = std::fopen(filepath.c_str(), "rb");
    if(file == NULL) {
        log_error("Cannot open file %s", filepath.c_str());
        throw std::invalid_argument("Cannot open file");
    }

    uint64_t fileSize = fs::file_size(filepath);

    std::vector<uint8_t> fileData(fileSize);

    int read = std::fread(&fileData[0], 1, fileSize, file);
    if(read != fileSize) {
        log_error("Reported file size %d not read, actually read %d", fileSize, read);
    }
    return fileData;
}

std::vector<uint8_t> FileProvider::readArchiveFile(const std::string& filepath)
{
    std::string lowerCased = filepath;
    std::transform(filepath.begin(), filepath.end(), lowerCased.begin(), [](unsigned char c) { return std::tolower(c); });

    log_debug("Searching for file in archive: %s", lowerCased.c_str());

    BSA::BSA* containingArchive = getContainingArchive(lowerCased);

    if (containingArchive == NULL) {
        log_error("Cannot find file %s in any archive", filepath.c_str());
        throw std::invalid_argument("Cannot open file");
    }

    log_debug("Found archived file: %s", lowerCased.c_str());
    return containingArchive->getFile(lowerCased);
}

BSA::BSA* FileProvider::getContainingArchive(const std::string& filepath)
{

    BSA::BSA* containingArchive = NULL;

    log_debug("Searching for archived file: %s", filepath.c_str());
    log_debug("Checking path cache.");

    auto archiveIt = this->archivePathCache.find(filepath);
    if (archiveIt != this->archivePathCache.end()) {
        log_debug("Hit! %s", archiveIt->second->getName().c_str());
        containingArchive = archiveIt->second;

    } else {

        for (auto archive : archives) {

            log_debug("Searching %s", archive->getName().c_str());
            if (archive->fileExists(filepath)) {

                log_debug("Found!");
                this->archivePathCache.insert(std::make_pair(filepath, /*static_cast<BSA::BSA*>(*/ archive /*)*/));
                containingArchive = archive;

                break;
            }
        }
    }

    return containingArchive;
}