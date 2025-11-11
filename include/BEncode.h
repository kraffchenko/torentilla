#ifndef BENCODE_H
#define BENCODE_H

#include <iostream>
#include <vector>
#include <string_view>
#include <stdexcept>
#include "BValue.h"

class BEncode{
public:
  void encodeObject();
  std::vector<std::byte> encodeObject(const BValue&);
  std::vector<std::byte> encodeDict(const std::map<std::string, BValue>&);
private:
  std::byte m_integer_start{105}; // ASCII I 
  std::byte m_end_delimeter{101}; // ASCII E
  std::byte m_list_start{108}; // ASCII L 
  std::byte m_dict_start{100}; // ASCII D
 std::vector<std::byte> encode64Int(const int64_t&);
  std::vector<std::byte> encodeString(const std::string&);
  std::vector<std::byte> encodeList(const std::vector<BValue>&);
 

};

#endif

