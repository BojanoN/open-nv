#pragma once

#include "esm/reader.hpp"
#include "esm/types.hpp"
#include "logc/log.h"
#include <cassert>
#include <sstream>
#include <string>
#include <unordered_map>

namespace GameWorld {

class GameDataBase {
public:
    virtual void    load(ESM::ESMReader& reader) = 0;
    virtual ssize_t size()                       = 0;
};

template <class T>
class GameData : public GameDataBase {
private:
    std::unordered_map<formid, T*> dataMap;
    void                           raiseError(formid id) const;

public:
    virtual ssize_t size() { return dataMap.size(); }
    virtual void    load(ESM::ESMReader& reader);
    const T&        get(formid id) const;
    void            insert(T& data);
};

template <class T>
const T& GameData<T>::get(formid id) const
{
    typename std::unordered_map<formid, T*>::const_iterator it = dataMap.find(id);

    if (it == dataMap.end()) {
        raiseError(id);
    }
    return *(it->second);
}

template <class T>
void GameData<T>::insert(T& record)
{
    dataMap.insert(std::pair<formid, T*>(record.id, &record));
}

template <class T>
void GameData<T>::load(ESM::ESMReader& reader)
{
    try {
        T record(reader);
        this->insert(record);
        assert(dataMap.find(record.id) != dataMap.end());
    } catch (std::runtime_error e) {
        std::stringstream s;
        log_fatal("Cannot read record!");
        log_fatal(e.what());
        throw std::runtime_error(e.what());
    }
}

template <class T>
void GameData<T>::raiseError(formid id) const
{
    std::stringstream s;
    s << "Data error: no data loaded with formId " << id << '\n';
    throw std::out_of_range(s.str());
}
};
