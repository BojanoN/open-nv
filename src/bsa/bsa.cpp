#include "bsa.hpp"
#include "hash.hpp"
#include "logc/log.h"
#include "util/compress.hpp"
#include <algorithm>
#include <string>

#define SIZE_COMPRESSION_MASK 0x40000000

namespace BSA {
BSA::BSA(std::string path)
    : file(path.c_str())
{
    this->file.read(reinterpret_cast<char*>(&this->header), sizeof(Header));

    if (this->header.fileId != BSAId) {
        throw std::runtime_error("Not a valid BSA file: " + path);
    }

    this->file.seekg(this->header.folderRecordsOffset, std::ios::beg);
    for (uint32_t i = 0; i < this->header.folderCount; i++) {
        this->file.read(reinterpret_cast<char*>(&this->folders.emplace_back()), sizeof(FolderRecord));
    };

    this->compressedByDefault = this->header.archiveFlags & ArchiveFlags::CompressedArchive;
    this->filenamesEmbedded   = this->header.archiveFlags & ArchiveFlags::EmbedFileNames;
    this->folderNamesIncluded = this->header.archiveFlags & ArchiveFlags::IncludeDirectoryNames;

    log_info("Read %d folders", this->folders.size());
    // TODO: dynamically determine fair cache size
    this->cache = new BSACache(50);
};

bool BSACache::insert(uint64_t key, uint32_t value) const
{
    return false;
}

uint32_t BSACache::get(uint64_t key) const
{
    return 0;
}

bool BSACache::exists(uint64_t key) const
{
    return false;
}

// TODO: binary searches for files and folders
std::vector<uint8_t>* BSA::getFile(std::string path)
{

    FolderRecord* folder;
    FileRecord    file;

    uint32_t    lastBackslash = path.find_last_of('\\');
    std::string folderName    = path.substr(0, lastBackslash);
    std::string fileName      = path.substr(lastBackslash + 1);

    uint64_t folderHval = FNVHash(folderName, "");
    uint64_t fileHval   = FNVHash("", fileName);
    uint64_t cacheKey   = folderHval + fileHval;

    uint32_t offsetToFile = 0;

    bool folderFound  = false;
    bool fileFound    = false;
    bool isCompressed = false;

    for (uint32_t i = 0; i < this->folders.size(); i++) {
        if (this->folders[i].nameHash == folderHval) {
            folder      = &this->folders[i];
            folderFound = true;
        }
    }

    if (!folderFound) {
        log_warn("File %s not found", path.c_str());
        return nullptr;
    }

    log_debug("Folder found!");

    if (!this->cache->exists(cacheKey)) {

        uint32_t offset = folder->offset - this->header.totalFileNameLength;

        this->file.seekg(offset, std::ios::beg);

        if (this->folderNamesIncluded) {
            std::string folder;
            uint8_t     size;
            this->file.read(reinterpret_cast<char*>(&size), 1);
            folder.resize(size);
            this->file.read(folder.data(), size);
            log_debug("Folder name: %s", folder.c_str());
        }

        for (uint32_t i = 0; i < folder->fileCount; i++) {
            this->file.read(reinterpret_cast<char*>(&file), sizeof(FileRecord));
            if (file.nameHash == fileHval) {
                fileFound = true;
                break;
            }
        }

        offsetToFile = file.offsetToFile;

    } else {
        offsetToFile = this->cache->get(cacheKey);
        fileFound    = true;
    }

    if (!fileFound) {
        log_warn("File %s not found", path.c_str());
        return nullptr;
    }

    log_debug("File found!");
    this->file.seekg(offsetToFile, std::ios::beg);

    std::vector<uint8_t>* ret = nullptr;

    isCompressed = this->compressedByDefault ^ (bool)(file.size & SIZE_COMPRESSION_MASK);

    if (isCompressed) {
        log_debug("Decompressing file..");
        file.size ^= SIZE_COMPRESSION_MASK;
        uint32_t decompSize;

        if (this->filenamesEmbedded) {
            uint8_t fnameSize;
            this->file.read(reinterpret_cast<char*>(&fnameSize), 1);
            this->file.seekg(fnameSize, std::ios::cur);
        }

        this->file.read(reinterpret_cast<char*>(&decompSize), sizeof(uint32_t));
        std::vector<uint8_t>* tmpBuf = new std::vector<uint8_t>(file.size);
        ret                          = new std::vector<uint8_t>(decompSize);

        Util::zlibDecompress(*tmpBuf, *ret);

        delete tmpBuf;
    } else {
        ret = new std::vector<uint8_t>(file.size);

        this->file.read(reinterpret_cast<char*>(ret->data()), file.size);
    }

    return ret;
};

};
