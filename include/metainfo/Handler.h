#ifndef METAINFOHANDLER_H
#define METAINFOHANDLER_H

#include <iostream>
#include <string_view>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>

#include "bencode/Value.h"
#include "bencode/Decode.h"
#include "bencode/Encode.h"
#include "Data.h"
#include "torrent/Config.h"
#include <boost/compute/detail/sha1.hpp>  
#include <openssl/evp.h>

namespace metainfo{
  class Handler{
  public:
    Data createMetainfo(std::string& path);
    Data createDotTorrent(torrent::Config& config);
  private:
    template<typename T>
    T getValue(const std::map<std::string, bencode::Value>&, const std::string& key);
    template<typename T>
    std::optional<T> getValueOpt(const std::map<std::string, bencode::Value>&, const std::string& key);
    std::optional<std::vector<std::vector<std::string>>> getValueOptList(const std::map<std::string, bencode::Value>&, const std::string& key);
    std::array<std::byte, 20> returnSHA1(std::vector<std::byte>& data);
    std::string createPiecesString(std::ifstream& file_ref);
    void addPieceString(std::string& pieces, std::ifstream& file_ref, int byte_array_size);
    void parseElement(size_t pos, std::vector<std::byte>&);
    size_t addString(size_t pos, std::vector<std::byte>&);
    size_t addInt(size_t pos, std::vector<std::byte>&);
    size_t addList(size_t pos, std::vector<std::byte>&);
    size_t addDict(size_t pos, std::vector<std::byte>&);
  };


  template<typename T>
  T Handler::getValue(const std::map<std::string, bencode::Value>& map_ref, const std::string& key){

    if(map_ref.count(key) == 0){
      throw std::invalid_argument("MetainfoHandler::getValue: Map does not contain a neccessary key");
    }else{
     return std::get<T>(map_ref.at(key).value); 
    }
  }

  template<typename T>
  std::optional<T> Handler::getValueOpt(const std::map<std::string, bencode::Value>& map_ref, const std::string& key){
    if(map_ref.count(key) == 0){
      return std::nullopt;
    }else{
      return std::get<T>(map_ref.at(key).value);
    }
  }

  template<>
  inline std::optional<std::vector<std::vector<std::string>>> Handler::getValueOpt<std::vector<std::vector<std::string>>> (const std::map<std::string, bencode::Value>& map_ref, const std::string& key){
   if(map_ref.count(key) == 0){
      return std::nullopt;
    }else{
      std::vector<std::vector<std::string>> announces_list{};
      const std::vector<bencode::Value>& bvalues_list {std::get<std::vector<bencode::Value>>(map_ref.at(key).value)};
      announces_list.reserve(std::size(bvalues_list));
      for (const bencode::Value& bvalue_announce_list : bvalues_list){
        const std::vector<bencode::Value>& inner_bvalue_announce_list{std::get<std::vector<bencode::Value>>(bvalue_announce_list.value)};
        std::vector<std::string> inner_announce_list{};
        for(const bencode::Value& announce_string : inner_bvalue_announce_list){
          inner_announce_list.push_back(std::move(std::get<std::string>(announce_string.value)));
        }
        announces_list.push_back(std::move(inner_announce_list));
      }
      return announces_list;
    }
  }
}

#endif
