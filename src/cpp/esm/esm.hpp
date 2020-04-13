#pragma once
#include "record.hpp"
#include "subrecord.hpp"

#include "record.hpp"
#include <unordered_map>

namespace ESM {

class RecordLoader {
public:
    virtual Record* operator()(FILE* esm_file) = 0;
};
template <typename T>
class TypeRecordLoader : public RecordLoader {
public:
    T* operator()(FILE* esm_file) { return new T(esm_file); };
};

class ESMReader {
public:
    ESMReader(std::string& path)
    {

        this->file = std::fopen(path.c_str(), "rb");
        if (this->file == NULL) {
            exit(1);
        }
    };
    ~ESMReader() {};

    bool hasMoreSubrecords();
    bool hasMoreRecords() { return this->fileLeft > 0; };
    void skipRecord();

    RecordHeader&    getCurrentRecord() { return currentRecordHead; };
    SubrecordHeader& getCurrentSubrecord() { return currentSubrecordHead; };

    void readNextSubrecord();
    void readNextRecord();

    template <typename T>
    void readType(T* dest);

    std::string* readString();

    Record* nextRecord();

private:
    FILE*   file;
    ssize_t fileSize;

    ssize_t fileLeft;
    ssize_t recordLeft;

    RecordHeader    currentRecordHead;
    SubrecordHeader currentSubrecordHead;

    static std::unordered_map<std::string, RecordLoader*> recordLoaders;
};

class ESM {
public:
    ESM(std::string& path);
    ~ESM();

private:
    ESMReader                             reader;
    std::unordered_map<uint32_t, Record*> records;
};

};
