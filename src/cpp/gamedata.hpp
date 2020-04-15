#include <string>
#include <unordered_map>
#include <sstream>
#include "esm/types.hpp"
#include "esm/esm.hpp"

namespace GameWorld {

class GameDataBase {

  virtual void load(ESM::ESMReader& reader) = 0;
};


template <class T>
class GameData : public GameDataBase {
 private:
  std::unordered_map<formid, T*> dataMap;
  void raiseError(formid id) const;

 public:
  // std::string getType();
  virtual void load(ESM::ESMReader& reader);
  const T& get(formid id) const;
  void insert(T& data);
};

template <class T>
const T& GameData<T>::get(formid id) const {
  typename std::unordered_map<formid, T*>::const_iterator it = dataMap.find(id);

  if (it == dataMap.end()) {
    raiseError(id);
  }
  return *(it->second);
}

template <class T>
void GameData<T>::insert(T& record) {
  dataMap.insert(std::pair<formid, T*>(record.id, &record));
}

template <class T>
void GameData<T>::load(ESM::ESMReader& reader) {
  T record;
  record.load(reader);
  this->insert(record);
}

template <class T>
void GameData<T>::raiseError(formid id) const {
  std::stringstream s;
  s << "Data error: no data loaded with formId " << id << '\n';
  throw std::out_of_range(s.str());
}
};