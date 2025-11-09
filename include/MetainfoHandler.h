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
  std::optional<std::vector<std::vector<std::string>>> getValueOptList(const std::map<std::string, BValue>&, const std::string& key);
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

template<>
inline std::optional<std::vector<std::vector<std::string>>> MetainfoHandler::getValueOpt<std::vector<std::vector<std::string>>> (const std::map<std::string, BValue>& map_ref, const std::string& key){
 if(map_ref.count(key) == 0){
    return std::nullopt;
  }else{
    std::vector<std::vector<std::string>> announces_list{};
    const std::vector<BValue>& bvalues_list {std::get<std::vector<BValue>>(map_ref.at(key).value)};
    announces_list.reserve(std::size(bvalues_list));
    for (const BValue& bvalue_announce_list : bvalues_list){
      const std::vector<BValue>& inner_bvalue_announce_list{std::get<std::vector<BValue>>(bvalue_announce_list.value)};
      std::vector<std::string> inner_announce_list{};
      for(const BValue& announce_string : inner_bvalue_announce_list){
        inner_announce_list.push_back(std::move(std::get<std::string>(announce_string.value)));
      }
      announces_list.push_back(std::move(inner_announce_list));
    }
    return announces_list;
  }
}


#endif
