#ifndef METAINFOHANDLER_H
#define METAINFOHANDLER_H

#include <iostream>
#include <string_view>
#include <vector>
#include <map>
#include <fstream>

#include "BValue.h"
#include "BDecode.h"
#include "BEncode.h"
#include "Metainfo.h"

class MetainfoHandler{
public:
  Metainfo createMetainfo(std::string& path);
private:
  template<typename T>
  T getValue(std::map<std::string, BValue>&, std::string_view);

};


template<typename T>
T MetainfoHandler::getValue(std::map<std::string, BValue>& map_ref, std::string_view key){
  std::vector<std::string> optional_keys{};
  if(map_ref.count(key) == 0){
    if (optional_keys.contains(key)){
      return std::nullopt;
    }else{
      throw std::invalid_argument("Key doesnt exist");
    }
  }else{
   return map_ref.at(key); 
  }
} 

#endif
