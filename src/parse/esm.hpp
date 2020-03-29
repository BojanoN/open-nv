#include <string>
#include <unordered_map>
#include <fstream>
#include "data_file.hpp"

class ESM{

public:
  ESM(const char* path): name(path){
    std::ifstream file{path, std::ios::binary};
    char type[4] = {0};

    while( file.read(type, 4)){
      switch(){
        // Handle records
      }
    }

  };

  ~ESM(){};

  std::string name;
};
