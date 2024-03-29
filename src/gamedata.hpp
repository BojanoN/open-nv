#pragma once

#include "esm/reader.hpp"
#include "esm/record.hpp"
#include "esm/types.hpp"
#include "logc/log.h"
#include <cassert>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

namespace GameWorld {

class GameDataBase {
public:
    virtual ESM::Record* load(ESM::ESMReader& reader) = 0;
    virtual ssize_t      size()                       = 0;
    virtual formid       get(std::string& editorId)   = 0;
    virtual ESM::Record* getBase(formid id)           = 0;

    virtual ~GameDataBase() { }
};

template <class T>
class GameData : public GameDataBase {
private:
    //std::unordered_map<formid, std::shared_ptr<T>> dataMap;
    std::unordered_map<formid, T*>          dataMap;
    std::unordered_map<std::string, formid> editorIdMap;
    void                                    raiseError(formid id) const;

public:
    virtual ssize_t                          size() { return dataMap.size(); }
    virtual ESM::Record*                     load(ESM::ESMReader& reader);
    std::unordered_map<formid, T*>&          getMap() { return this->dataMap; };
    std::unordered_map<std::string, formid>& getEdidMap() { return this->editorIdMap; };

    T*           get(formid id);
    ESM::Record* getBase(formid id);
    formid       get(std::string& editorId);
    void         insert(T* data);
    virtual ~GameData();
};

template <class T>
GameData<T>::~GameData()
{
    for (auto&& it = dataMap.begin(); it != dataMap.end(); it++) {
        delete it->second;
    }
}

template <class T>
T* GameData<T>::get(formid id)
{
    typename std::unordered_map<formid, T*>::const_iterator it = dataMap.find(id);

    if (it == dataMap.end()) {
        return nullptr;
    }
    return it->second;
}

template <class T>
ESM::Record* GameData<T>::getBase(formid id)
{
    typename std::unordered_map<formid, T*>::const_iterator it = dataMap.find(id);

    if (it == dataMap.end()) {
        return nullptr;
    }
    return it->second;
}

template <class T>
formid GameData<T>::get(std::string& editorId)
{
    auto it = editorIdMap.find(editorId);

    if (it != editorIdMap.end()) {
        return it->second;
    }

    return 0;
}

template <class T>
void GameData<T>::insert(T* record)
{
    dataMap.insert_or_assign(record->id, record);
    if (record->editorId.size()) {
        editorIdMap.insert_or_assign(record->editorId, record->id);
    }
}

template <class T>
ESM::Record* GameData<T>::load(ESM::ESMReader& reader)
{
    //log_debug("Fpointer before load 0x%x", reader.getCurrentPosition());
    try {
        T* record = new T(reader);
        this->insert(record);
        assert(dataMap.find(record->id) != dataMap.end());
        return reinterpret_cast<ESM::Record*>(record);
    } catch (std::runtime_error& e) {
        std::stringstream s;
        log_fatal("Cannot read record!");
        log_fatal(e.what());
        throw std::runtime_error(e.what());
    }
    //log_debug("Fpointer after load 0x%x", reader.getCurrentPosition());
}

template <class T>
void GameData<T>::raiseError(formid id) const
{
    std::stringstream s;
    s << "Data error: no data loaded with formId " << id << '\n';
    throw std::out_of_range(s.str());
}
};
