#pragma once

#include "data_file.hpp"


class TES4Record: public Record{

  TES4Record(std::ifstream& stream): Record(stream){
  
  }

};
