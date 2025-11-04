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
  T getValue(const std::map<std::string, BValue>&, const std::string& key);
  template<typename T>
  std::optional<T> getValueOpt(const std::map<std::string, BValue>&, const std::string& key);
};


template<typename T>
T MetainfoHandler::getValue(const std::map<std::string, BValue>& map_ref, const std::string& key){

  if(map_ref.count(key) == 0){
    throw std::invalid_argument("MetainfoHandler::getValue: Map does not contain a neccessary key");
  }else{
   return std::get<T>(map_ref.at(key).value); 
  }
}

template<typename T>
std::optional<T> MetainfoHandler::getValueOpt(const std::map<std::string, BValue>& map_ref, const std::string& key){
  if(map_ref.count(key) == 0){
    return std::nullopt;
  }else{
    return std::get<T>(map_ref.at(key).value);
  }
}

#endif
