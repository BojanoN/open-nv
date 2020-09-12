#include "bsa.hpp"
#include "../logc/log.h"
#include "../streams/bytearray/ibastream.hpp"
#include "../util/compress.hpp"
#include "hash.hpp"
#include <algorithm>
#include <string>
#include <utility>

#define SIZE_COMPRESSION_MASK 0x40000000
#define MIN_CACHE_SIZE        50

namespace BSA {
BSA::BSA(InputStream* file, const std::string& name)
    : file { file }
    , name { name }
{
    this->file->read(reinterpret_cast<char*>(&this->header), sizeof(Header), 1);

    if (this->header.fileId != BSAId) {
        throw std::runtime_error("Not a valid BSA file!");
    }

    this->file->inputSeek(this->header.folderRecordsOffset, StreamPosition::beg);
    for (uint32_t i = 0; i < this->header.folderCount; i++) {
        this->file->read(reinterpret_cast<char*>(&this->folders.emplace_back()), sizeof(FolderRecord), 1);
    };

    this->compressedByDefault = this->header.archiveFlags & ArchiveFlags::CompressedArchive;
    this->filenamesEmbedded   = this->header.archiveFlags & ArchiveFlags::EmbedFileNames;
    this->folderNamesIncluded = this->header.archiveFlags & ArchiveFlags::IncludeDirectoryNames;
    this->fileNamesIncluded   = this->header.archiveFlags & ArchiveFlags::IncludeFileNames;

    if (this->fileNamesIncluded) {
        uint32_t fileNamesOffset = (this->header.fileCount * (sizeof(FileRecord))) + (this->folderNamesIncluded ? (this->header.folderCount + this->header.totalFolderNameLength) : 0);
        this->fileNames          = new std::unordered_set<std::string>();

        this->file->inputSeek(fileNamesOffset, StreamPosition::cur);
        log_debug("OFFSET: %u", this->file->inputTell());

        char* buffer = new char[this->header.totalFileNameLength];
        this->file->read(buffer, this->header.totalFileNameLength, 1);

        int   read = 0;
        char* tmp  = buffer;

        while (read < this->header.totalFileNameLength) {
            std::string entry(tmp);
            this->fileNames->insert(entry);

            tmp += entry.size() + 1;
            read += entry.size() + 1;
        }

        delete buffer;
    }

    log_info("Read %d folders", this->folders.size());
    log_info("Compressed by default: %s", this->compressedByDefault ? "true" : "false");
    log_info("FilenameBlock included: %s", this->fileNamesIncluded ? "true" : "false");
    // TODO: dynamically determine fair cache size
    uint32_t dynSize = (this->header.fileCount * 0.1);
    uint32_t size    = (dynSize < MIN_CACHE_SIZE) ? MIN_CACHE_SIZE : dynSize;
    log_debug("Cache size: %u", size);
    this->cache = std::make_unique<BSACache>(size);
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

    this->file->inputSeek(folderOffset, StreamPosition::beg);

    if (this->folderNamesIncluded) {
        uint8_t size;
        this->file->read(reinterpret_cast<char*>(&size), 1, 1);
        this->file->inputSeek(size, StreamPosition::cur);
    }

    uint32_t startOffset = this->file->inputTell();
    int32_t  l           = 0;
    int32_t  r           = folder->fileCount - 1;
    int32_t  mid         = 0;

    FileRecord tmp;

    while (l <= r) {
        mid = (l + r) / 2;

        this->file->inputSeek(startOffset + (sizeof(FileRecord) * mid), StreamPosition::beg);
        this->file->read(reinterpret_cast<char*>(&tmp), sizeof(FileRecord), 1);

        if (tmp.nameHash < hashval) {
            l = mid + 1;
        } else if (tmp.nameHash > hashval) {
            r = mid - 1;
        } else {
            ret.first  = tmp;
            ret.second = true;
            log_debug("File record found at 0x%x!", (int)this->file->inputTell() - sizeof(FileRecord));
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
        return this->fileNames->count(fileName);
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

//std::vector<uint8_t>* BSA::getFile(std::string path)
//std:unique_ptr<InputStream> BSA::getFile(const std::string& path)
InputStream* BSA::getFile(const std::string& path)
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
            log_warn("Folder %s not found", folderName.c_str());
            return nullptr;
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
        log_warn("File %s not found", path.c_str());
        return nullptr;
    }

    offsetToFile = file.offsetToFile;

    log_debug("File found at 0x%x!", offsetToFile);
    this->file->inputSeek(offsetToFile, StreamPosition::beg);

    std::vector<uint8_t>* ret = nullptr;

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
            this->file->read(reinterpret_cast<char*>(&fnameSize), 1, 1);
            this->file->inputSeek(fnameSize, StreamPosition::cur);
        }

        this->file->read(reinterpret_cast<char*>(&decompSize), sizeof(uint32_t), 1);

        // file.size + 4, what the fuck
        log_debug("Compressed size: %d + 4", file.size);
        std::vector<uint8_t>* tmpBuf = new std::vector<uint8_t>(file.size + 4);
        this->file->read(reinterpret_cast<char*>(tmpBuf->data()), 1, file.size + 4);

        log_debug("Uncompressed size: %d", decompSize);
        ret = new std::vector<uint8_t>(decompSize);

        try {
            Util::zlibDecompress(*tmpBuf, *ret);

            if (ret->size() != decompSize) {
                throw std::runtime_error("Wrong uncompressed size: " + std::to_string(ret->size()) + ", expected: " + std::to_string(decompSize));
            }

            delete tmpBuf;
        } catch (std::runtime_error& e) {
            log_fatal("%s", e.what());

            delete tmpBuf;
            delete ret;

            return nullptr;
        }

    } else {
        ret = new std::vector<uint8_t>(file.size);

        this->file->read(reinterpret_cast<char*>(ret->data()), 1, file.size);
    }

    //return std::make_unique<ByteArrayInputStream>(ret);
    return new ByteArrayInputStream(ret);
};
};
