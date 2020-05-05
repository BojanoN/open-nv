#include "util/enumflags.hpp"
#include <cstdint>
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
    uint8_t      fileId[4]; // Always BSA0
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
    std::string name; // Only present if EmbedFileNames is set
    uint64_t    nameHash;
    uint32_t    size; // 0x40000000 controls compression
    uint32_t    offsetToFile;
};

struct FilenameBlock {
    std::vector<std::string> filenames;
};

struct CompressedFileBlock {
    std::string          name;
    uint32_t             uncompressedSize;
    std::vector<uint8_t> data;
};

struct UncompressedFileBlock {
    std::string          name;
    std::vector<uint8_t> data;
};

};
ENABLE_BITWISE_OPERATORS(BSA::ArchiveFlags);
ENABLE_BITWISE_OPERATORS(BSA::FileFlags);
