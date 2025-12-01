#ifndef BDECODE_H
#define BDECODE_H


#include <iostream>
#include <cstddef>
#include <array>
#include <vector>
#include <sstream>
#include <string_view>
#include <stdexcept>
#include "Value.h"

namespace bencode{
  class Decode{
  public:
    Decode(std::vector<std::byte>&);  
    Value parseByteArray();
    std::pair<size_t, Value> parseBValue(size_t);
  private:
    std::vector<std::byte>& m_byte_array;
    std::byte m_integer_start{105}; // ASCII I 
    std::byte m_end_delimeter{101}; // ASCII E
    std::byte m_list_start{108}; // ASCII L 
    std::byte m_dict_start{100}; // ASCII D


    std::pair<size_t, Value>  parseBDict(size_t);
    std::pair<size_t, Value>  parseBList(size_t);
    std::pair<size_t, Value>  parseBInt(size_t);
    std::pair<size_t, Value>  parseBString(size_t);
    int returnRange(std::string_view);
    bool isString(std::byte);
    bool intIsValid(size_t, size_t);
  };
}
#endif
