#pragma once

#include "util/enumflags.hpp"
#include <cstdint>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>

#define BSAId 0x00415342

namespace BSA {

enum class ArchiveFlags : uint32_t {
    IncludeDirectoryNames      = 0x001,
    IncludeFileNames           = 0x002,
    CompressedArchive          = 0x004,
    RetainDirectoryNames       = 0x008,
    RetainFileNames            = 0x010,
    RetainFileNamesOffsets     = 0x020,
    Xbox360Archive             = 0x040,
    RetainStringsDuringStartup = 0x080,
    EmbedFileNames             = 0x100,
    XMemCodec                  = 0x200
};

enum class FileFlags : uint32_t {
    Meshes        = 0x001,
    Textures      = 0x002,
    Menus         = 0x004,
    Sounds        = 0x008,
    Voices        = 0x010,
    Shaders       = 0x020,
    Trees         = 0x040,
    Fonts         = 0x080,
    Miscellaneous = 0x100,
};

struct Header {
    uint32_t     fileId; // Always BSA0
    uint32_t     version; // 0x68 for FNV
    uint32_t     folderRecordsOffset;
    ArchiveFlags archiveFlags;
    uint32_t     folderCount;
    uint32_t     fileCount;
    uint32_t     totalFolderNameLength;
    uint32_t     totalFileNameLength;
    FileFlags    fileFlags;
};

struct FolderRecord {
    uint64_t nameHash;
    uint32_t fileCount;
    uint32_t offset;
};

struct FileRecord {
    uint64_t nameHash;
    uint32_t size; // 0x40000000 controls compression
    uint32_t offsetToFile;
};

struct FilenameBlock {
    std::vector<std::string> filenames;
};

struct UncompressedFileBlock {
    std::string          name;
    std::vector<uint8_t> data;
};

class BSACache {

public:
    bool                        insert(uint64_t key, FileRecord value);
    bool                        exists(uint64_t key);
    std::pair<FileRecord, bool> get(uint64_t key);
    void                        clear();

    BSACache(uint32_t size)
        : cacheEntries(size)
        , frequency()
        , lfu()
        , maxSize(size) {};

private:
    std::unordered_map<uint64_t, FileRecord>                                  cacheEntries;
    std::multimap<uint64_t, uint64_t>                                         frequency;
    std::unordered_map<uint64_t, std::multimap<uint64_t, uint64_t>::iterator> lfu;
    uint32_t                                                                  maxSize;
};

class BSA {
public:
    BSA(std::string path);
    ~BSA() { delete cache; };

    std::vector<uint8_t>* getFile(std::string path);
    bool                  fileExists(std::string path);

private:
    std::ifstream file;
    Header        header;
    BSACache*     cache;
    // hash, offset
    std::vector<FolderRecord> folders;

    std::pair<FolderRecord*, bool> findFolder(uint64_t hashval);
    std::pair<FileRecord, bool>    findFile(FolderRecord* folder, uint64_t hashval);

    bool compressedByDefault;
    bool filenamesEmbedded;
    bool folderNamesIncluded;
};

};
ENABLE_BITWISE_OPERATORS(BSA::ArchiveFlags);
ENABLE_BITWISE_OPERATORS(BSA::FileFlags);