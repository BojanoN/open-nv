#include "bsa.hpp"
#include "hash.hpp"
#include "logc/log.h"
#include <algorithm>
#include <iostream>
#include <string>

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

    std::cout << this->compressedByDefault << " " << this->filenamesEmbedded << "\n";

    log_info("Read %d folders", this->folders.size());
};

std::vector<uint8_t>* BSA::getFile(std::string path)
{

    uint32_t    lastBackslash = path.find_last_of('\\');
    uint32_t    first         = path.find("\\");
    std::string folderName    = path.substr(0, lastBackslash);
    std::string fileName      = path.substr(lastBackslash + 1);

    uint64_t folderHval = FNVHashPair(folderName, "");
    uint32_t i          = -1;

    log_debug("%s", folderName.c_str());
    log_debug("%lld", folderHval);

    for (i = 0; i < this->folders.size(); i++) {
        if (this->folders[i].nameHash == folderHval) {
            break;
        }
    }

    log_debug("Folder found at %d", i);
    log_debug("%lld", this->folders[0].nameHash);

    uint32_t offset = this->folders[i].offset - this->header.totalFileNameLength;

    this->file.seekg(offset, std::ios::beg);

    if (this->folderNamesIncluded) {
        std::string folder;
        uint8_t     size;
        this->file.read(reinterpret_cast<char*>(&size), 1);
        folder.resize(size);
        this->file.read(folder.data(), size);
        log_debug("Folder name: %s", folder.c_str());
    }

    return nullptr;
};

};
