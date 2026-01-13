#ifndef METADATA_H
#define METADATA_H

#include <iostream>
#include <vector>
#include <optional>
#include <random>
#include <array>
#include <map>
#include <fstream>
#include "bencode/Value.h"
#include "bencode/Decode.h"
#include "bencode/Encode.h"
#include "torrent/dottorrent/file.h"
#include <boost/compute/detail/sha1.hpp>  
#include <openssl/evp.h>

namespace torrent::dottorrent{
class Metadata {
  public:
    Metadata(std::string_view announce, int64_t piece_length, std::string_view pieces, std::string_view name,
              int64_t length, std::array<std::byte, 20> info_hash, std::optional<std::vector<std::vector<std::string>>> announce_list, 
              std::optional<int64_t> creation_date, std::optional<std::string_view> comment, std::optional<std::string_view> created_by, 
              std::optional<std::string_view> encoding,
              std::optional<int> is_private, std::optional<std::string_view> md5sum
            );
    Metadata() = delete;

    std::string_view getAnnounce() const { return m_announce; };
    int64_t getPieceLength() const { return m_piece_length; };
    std::string_view getPieces() const { return m_pieces; };
    std::string_view getName() const { return m_name; };
    int64_t getLength() const { return m_length; };
    std::array<std::byte, 20> getInfoHash() const { return m_info_hash; }; 

    std::optional<std::string_view> getComment() const { return m_comment; };
    std::optional<std::string_view> getEncoding() const { return m_encoding; };
    std::optional<std::string_view> getCreatedBy() const { return m_created_by; };
    std::optional<std::vector<std::vector<std::string>>> getAnnounceList() const { return m_announce_list;};
    std::optional<int> getPrivateStatus() const { return m_is_private; };
    std::optional<std::string_view> getMd5sum() const { return m_md5sum; };
    std::optional<int64_t> getCreationDate() { return m_creation_date; }


  private:
    const std::string m_announce{};  
    const std::string m_name{};
    const int64_t m_piece_length{};
    const std::string m_pieces{};
    const int64_t m_length{};
    const std::array<std::byte, 20> m_info_hash{};

    const std::optional<std::vector<std::vector<std::string>>> m_announce_list{};
    const std::optional<int64_t> m_creation_date{};
    const std::optional<std::string> m_comment{};
    const std::optional<std::string> m_created_by{};
    const std::optional<std::string> m_encoding{};
    const std::optional<int> m_is_private{};
    const std::optional<std::string> m_md5sum{};
  };
  Metadata fromDotTorrent(const std::string_view path);
  template<typename T>
  inline T getValue(const std::map<std::string, bencode::Value>& map_ref, const std::string& key){
    if(map_ref.count(key) == 0){
      throw std::invalid_argument("metainfo::getValue: Map does not contain a neccessary key");
    }else{
     return std::get<T>(map_ref.at(key).value); 
    }
  }

  template<typename T>
  inline std::optional<T> getValueOpt(const std::map<std::string, bencode::Value>& map_ref, const std::string& key){
    if(map_ref.count(key) == 0){
      return std::nullopt;
    }else{
      return std::get<T>(map_ref.at(key).value);
    }
  }

  template<>
  inline std::optional<std::vector<std::vector<std::string>>> getValueOpt<std::vector<std::vector<std::string>>> (const std::map<std::string, bencode::Value>& map_ref, const std::string& key){
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
