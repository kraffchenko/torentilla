#ifndef BDECODE_H
#define BDECODE_H


#include <iostream>
#include <cstddef>
#include <array>
#include <vector>
#include <sstream>
#include <string_view>
#include <stdexcept>
#include <BValue.h>


class BDecode{
public:
  BDecode(std::vector<std::byte>&);
  BValue parseEncodedData();
private:
  std::vector<std::byte>& m_byte_array;
  std::byte m_integer_start{105}; // ASCII I 
  std::byte m_end_delimeter{101}; // ASCII E
  std::byte m_list_start{108}; // ASCII L 
  std::byte m_dict_start{100}; // ASCII D


  std::pair<size_t, BValue>  parseBDict(size_t);
  std::pair<size_t, BValue>  parseBList(size_t);
  std::pair<size_t, BValue>  parseBInt(size_t);
  std::pair<size_t, BValue>  parseBString(size_t);
  int returnRange(std::string_view);
  bool isString(std::byte);
  std::pair<size_t, BValue> parseBValue(size_t);
  bool intIsValid(size_t, size_t);
};

#endif
