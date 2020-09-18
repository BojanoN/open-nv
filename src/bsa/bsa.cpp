#include "bsa.hpp"
#include "../logc/log.h"
#include "../util/compress.hpp"
#include "hash.hpp"
#include <algorithm>
#include <string>
#include <utility>

#define SIZE_COMPRESSION_MASK 0x40000000
#define MIN_CACHE_SIZE        50

namespace BSA {
BSA::BSA(const std::string& path, const std::string& name) :  name { name }
{
    file = std::fopen(path.c_str(), "rb");
    if(file == NULL) {
        log_error("Cannot open BSA file %s", path.c_str());
        throw std::runtime_error("Cannot open archive file");
    }

    std::fread(reinterpret_cast<char*>(&this->header), sizeof(Header), 1, file);

    if (this->header.fileId != BSAId) {
        throw std::runtime_error("Not a valid BSA file!");
    }

    std::fseek(file, this->header.folderRecordsOffset, SEEK_SET);
    for (uint32_t i = 0; i < this->header.folderCount; i++) {
        std::fread(reinterpret_cast<char*>(&this->folders.emplace_back()), sizeof(FolderRecord), 1, file);
    };

    this->compressedByDefault = this->header.archiveFlags & ArchiveFlags::CompressedArchive;
    this->filenamesEmbedded   = this->header.archiveFlags & ArchiveFlags::EmbedFileNames;
    this->folderNamesIncluded = this->header.archiveFlags & ArchiveFlags::IncludeDirectoryNames;
    this->fileNamesIncluded   = this->header.archiveFlags & ArchiveFlags::IncludeFileNames;

    if (this->fileNamesIncluded) {
        uint32_t fileNamesOffset = (this->header.fileCount * (sizeof(FileRecord))) + (this->folderNamesIncluded ? (this->header.folderCount + this->header.totalFolderNameLength) : 0);

        std::fseek(file, fileNamesOffset, SEEK_CUR);
        log_debug("OFFSET: %u", std::ftell(file));

        std::vector<char> nameBuffer(this->header.totalFileNameLength);
        std::fread(&nameBuffer[0], this->header.totalFileNameLength, 1, file);

        int   read = 0;
        char*  currentNamePtr = &nameBuffer[0];

        while (read < this->header.totalFileNameLength) {
            std::string entry(currentNamePtr);
            this->fileNames.insert(entry);

            currentNamePtr += entry.size() + 1;
            read += entry.size() + 1;
        }

        //delete[] buffer;
    }

    log_info("Read %d folders", this->folders.size());
    log_info("Compressed by default: %s", this->compressedByDefault ? "true" : "false");
    log_info("FilenameBlock included: %s", this->fileNamesIncluded ? "true" : "false");
    // TODO: dynamically determine fair cache size
    uint32_t dynSize = (this->header.fileCount * 0.1);
    uint32_t size    = (dynSize < MIN_CACHE_SIZE) ? MIN_CACHE_SIZE : dynSize;
    log_debug("Cache size: %u", size);
    this->cache = new BSACache(size);
};

bool BSACache::insert(uint64_t key, FileRecord value)
{
    bool ret = this->cacheEntries.insert(std::make_pair(key, value)).second;

    if (ret) {
        if (this->cacheEntries.size() >= this->maxSize) {
            auto it = this->frequency.begin();

            this->cacheEntries.erase(it->first);
            this->lfu.erase(it->second);
            this->frequency.erase(it);
        }
        log_debug("Caching element %llu", key);
        this->lfu[key] = this->frequency.emplace(1, key);
        this->cacheEntries.emplace(key, value);
        log_debug("Current cache size: %u", this->cacheEntries.size());
    }

    return ret;
}

std::pair<FileRecord, bool> BSACache::get(uint64_t key)
{
    std::pair<FileRecord, bool> ret;
    auto                        found = this->cacheEntries.find(key);

    if (found != this->cacheEntries.end()) {
        ret.first  = found->second;
        ret.second = true;

        auto keyFreq = this->lfu[key];
        log_debug("Cache value before: %d", keyFreq->first);
        auto newPair = std::make_pair(keyFreq->first + 1, keyFreq->second);
        log_debug("Cache value after: %d", newPair.first);
        this->frequency.erase(keyFreq);

        lfu[key] = this->frequency.emplace(std::move(newPair));
    } else {
        ret.second = false;
    }

    return ret;
}

bool BSACache::exists(uint64_t key)
{
    return this->cacheEntries.count(key) != 0;
}

std::pair<FolderRecord*, bool> BSA::findFolder(uint64_t hashval)
{
    std::pair<FolderRecord*, bool> ret;
    FolderRecord                   folder;

    ret.first       = nullptr;
    folder.nameHash = hashval;

    auto first = std::lower_bound(this->folders.begin(), this->folders.end(), folder, [](FolderRecord l, FolderRecord r) { return l.nameHash < r.nameHash; });

    if (first != this->folders.end() && first->nameHash == hashval) {
        ret.second = true;
        ret.first  = &(*first);
    } else {
        ret.second = false;
    }

    return ret;
}

std::pair<FileRecord, bool> BSA::findFile(FolderRecord* folder, uint64_t hashval)
{
    std::pair<FileRecord, bool> ret;

    uint32_t folderOffset = folder->offset - this->header.totalFileNameLength;

    std::fseek(file, folderOffset, SEEK_SET);

    if (this->folderNamesIncluded) {
        uint8_t size;
        std::fread(reinterpret_cast<char*>(&size), 1, 1, file);
        std::fseek(file, size, SEEK_CUR);
    }

    uint32_t startOffset = std::ftell(file);
    int32_t  l           = 0;
    int32_t  r           = folder->fileCount - 1;
    int32_t  mid         = 0;

    FileRecord tmp;

    while (l <= r) {
        mid = (l + r) / 2;

        std::fseek(file, startOffset + (sizeof(FileRecord) * mid), SEEK_SET);
        std::fread(reinterpret_cast<char*>(&tmp), sizeof(FileRecord), 1, file);

        if (tmp.nameHash < hashval) {
            l = mid + 1;
        } else if (tmp.nameHash > hashval) {
            r = mid - 1;
        } else {
            ret.first  = tmp;
            ret.second = true;
            log_debug("File record found at 0x%x!", (int)std::ftell(file) - sizeof(FileRecord));
            return ret;
        }
    }

    ret.second = false;

    return ret;
};

void BSACache::clear()
{
    cacheEntries.clear();
    frequency.clear();
    lfu.clear();
}

bool BSA::fileExists(const std::string& path)
{
    int         lastBackslash = path.find_last_of('\\');
    std::string folderName;

    if (lastBackslash == std::string::npos) {
        lastBackslash = 0;
        folderName    = "";
    } else {
        folderName = path.substr(0, lastBackslash);
    }
    std::string fileName = path.substr(lastBackslash + 1);

    if (this->fileNamesIncluded) {
        return this->fileNames.count(fileName);
    } else {
        uint64_t folderHval = FNVHash(folderName, "");
        uint64_t fileHval   = FNVHash("", fileName);

        if (this->cache->exists(folderHval + fileHval)) {
            return true;
        }

        std::pair<FolderRecord*, bool> folderEntry = this->findFolder(folderHval);

        if (folderEntry.second) {
            std::pair<FileRecord, bool> fileEntry = this->findFile(folderEntry.first, fileHval);
            if (fileEntry.second) {
                this->cache->insert((folderHval + fileHval), fileEntry.first);
                return true;
            }
        }
    }
    return false;
}

std::vector<uint8_t> BSA::getFile(const std::string& path)
{

    FolderRecord* folder;
    FileRecord    file;

    int         lastBackslash = path.find_last_of('\\');
    std::string folderName;

    if (lastBackslash == std::string::npos) {
        lastBackslash = 0;
        folderName    = "";
    } else {
        folderName = path.substr(0, lastBackslash);
    }

    std::string fileName = path.substr(lastBackslash + 1);

    uint64_t folderHval = FNVHash(folderName, "");
    uint64_t fileHval   = FNVHash("", fileName);
    uint64_t cacheKey   = folderHval + fileHval;

    uint32_t offsetToFile = 0;

    bool fileFound    = false;
    bool isCompressed = false;

    std::pair<FileRecord, bool> cacheEntry = this->cache->get(cacheKey);

    if (!cacheEntry.second) {
        std::pair<FolderRecord*, bool> folderEntry = this->findFolder(folderHval);

        if (!folderEntry.second) {
            log_fatal("Folder %s not found", folderName.c_str());
            throw std::runtime_error("File in archive not found");
        }

        folder = folderEntry.first;
        log_debug("Folder found!");

        std::pair<FileRecord, bool> fileEntry = this->findFile(folder, fileHval);

        if (fileEntry.second) {
            file      = fileEntry.first;
            fileFound = true;

            this->cache->insert(cacheKey, file);
        }

    } else {
        file      = cacheEntry.first;
        fileFound = true;
    }

    if (!fileFound) {
        log_fatal("File %s not found", path.c_str());
        throw std::runtime_error("File in archive not found");
    }

    offsetToFile = file.offsetToFile;

    log_debug("File found at 0x%x!", offsetToFile);
    std::fseek(this->file, offsetToFile, SEEK_SET);

    bool sizeFlag = file.size & SIZE_COMPRESSION_MASK;
    if (sizeFlag) {
        file.size ^= SIZE_COMPRESSION_MASK;
    }

    isCompressed = this->compressedByDefault ^ sizeFlag;

    if (isCompressed) {
        log_debug("Decompressing file..");
        uint32_t decompSize;

        if (this->filenamesEmbedded) {
            uint8_t fnameSize;
            std::fread(reinterpret_cast<char*>(&fnameSize), 1, 1, this->file);
            std::fseek(this->file, fnameSize, SEEK_CUR);
        }

        std::fread(reinterpret_cast<char*>(&decompSize), sizeof(uint32_t), 1, this->file);

        // file.size + 4, what the fuck
        log_debug("Compressed size: %d + 4", file.size);
        std::vector<uint8_t> compressedFileData(file.size + 4);
        std::fread(reinterpret_cast<char*>(compressedFileData.data()), 1, file.size + 4, this->file);

        log_debug("Uncompressed size: %d", decompSize);
        std::vector<uint8_t> fileData(decompSize);

        try {
            Util::zlibDecompress(compressedFileData, fileData);

            if (fileData.size() != decompSize) {
                throw std::runtime_error("Wrong uncompressed size: " + std::to_string(fileData.size()) + ", expected: " + std::to_string(decompSize));
            }

        } catch (std::runtime_error& e) {
            log_fatal("%s", e.what());
        }

        return fileData;
    } 
    
    std::vector<uint8_t> fileData(file.size);
    std::fread(reinterpret_cast<char*>(fileData.data()), 1, file.size, this->file);
    return fileData;
};
};
