#include "bsa.hpp"
#include "hash.hpp"
#include "logc/log.h"
#include "util/compress.hpp"
#include <algorithm>
#include <string>
#include <utility>

#define SIZE_COMPRESSION_MASK 0x40000000
#define MIN_CACHE_SIZE        50

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

    log_info("File %s:", path.c_str());
    log_info("Read %d folders", this->folders.size());
    log_info("Compressed by default: %s", this->compressedByDefault ? "true" : "false");
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

    this->file.seekg(folderOffset, std::ios::beg);

    if (this->folderNamesIncluded) {
        uint8_t size;
        this->file.read(reinterpret_cast<char*>(&size), 1);
        this->file.seekg(size, std::ios::cur);
    }

    uint32_t startOffset = this->file.tellg();
    int32_t  l           = 0;
    int32_t  r           = folder->fileCount - 1;
    int32_t  mid         = 0;

    FileRecord tmp;

    while (l <= r) {
        mid = (l + r) / 2;

        this->file.seekg(startOffset + (sizeof(FileRecord) * mid), std::ios::beg);
        this->file.read(reinterpret_cast<char*>(&tmp), sizeof(FileRecord));

        if (tmp.nameHash < hashval) {
            l = mid + 1;
        } else if (tmp.nameHash > hashval) {
            r = mid - 1;
        } else {
            ret.first  = tmp;
            ret.second = true;
            return ret;
        }
    }

    ret.second = false;

    return ret;
};

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

    bool fileFound    = false;
    bool isCompressed = false;

    std::pair<FolderRecord*, bool> folderEntry = this->findFolder(folderHval);

    if (!folderEntry.second) {
        log_warn("Folder %s not found", folderName.c_str());
        return nullptr;
    }

    folder = folderEntry.first;
    log_debug("Folder found!");

    std::pair<FileRecord, bool> cacheEntry = this->cache->get(cacheKey);

    if (!cacheEntry.second) {
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
        log_warn("File %s not found", path.c_str());
        return nullptr;
    }

    offsetToFile = file.offsetToFile;

    log_debug("File found at 0x%x!", offsetToFile);
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

        try {
            Util::zlibDecompress(*tmpBuf, *ret);
            delete tmpBuf;
        } catch (std::runtime_error& e) {
            log_fatal("%s", e.what());
            delete tmpBuf;
            return ret;
        }

    } else {
        ret = new std::vector<uint8_t>(file.size);

        this->file.read(reinterpret_cast<char*>(ret->data()), file.size);
    }

    return ret;
};

};
