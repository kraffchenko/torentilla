#include "bencode/Encode.h"
namespace bencode{
  std::vector<std::byte> Encode::encode64Int(const int64_t& bint){
      std::vector<std::byte> bint_byte_array{};
      std::string integer_representation {"i"};
      integer_representation += std::to_string(bint);
      integer_representation += "e";
      for(char bchar : integer_representation){
         bint_byte_array.push_back(static_cast<std::byte>(bchar)); 
      }
      return bint_byte_array;
  };

  std::vector<std::byte> Encode::encodeString(const std::string& bstring){
      std::vector<std::byte> bstring_byte_array{};
      const size_t string_length{bstring.length()};
      std::string encoded_bstring {std::to_string(string_length) + ":" + bstring};
      for (char bchar : encoded_bstring){
        bstring_byte_array.push_back(static_cast<std::byte>(bchar));
      }
      return bstring_byte_array;
  };

  std::vector<std::byte> Encode::encodeList(const std::vector<Value>& blist){
      std::vector<std::byte> blist_byte_array{};
      blist_byte_array.push_back(static_cast<std::byte>('l'));
      for (const Value& bvalue : blist){
        std::vector<std::byte> bvalue_byte_array= encodeObject(bvalue);
        blist_byte_array.insert(blist_byte_array.end(), bvalue_byte_array.begin(), bvalue_byte_array.end());
      }
      blist_byte_array.push_back(static_cast<std::byte>('e'));
      return blist_byte_array;
  };

  std::vector<std::byte> Encode::encodeDict(const std::map<std::string, Value>& bmap){
    std::vector<std::byte> bmap_byte_array;
    bmap_byte_array.push_back(static_cast<std::byte>('d'));
    std::map<std::string, Value>::const_iterator it;
    for(it = bmap.begin(); it != bmap.end(); it++){
      std::vector<std::byte> bstring_byte_vector {encodeString(it->first)};
      std::vector<std::byte> bvalue_byte_vector {encodeObject(it->second)};
      bmap_byte_array.insert(bmap_byte_array.end(), bstring_byte_vector.begin(), bstring_byte_vector.end());
      bmap_byte_array.insert(bmap_byte_array.end(), bvalue_byte_vector.begin(), bvalue_byte_vector.end());
    }
    bmap_byte_array.push_back(static_cast<std::byte>('e'));
    return bmap_byte_array;

  }

  std::vector<std::byte> Encode::encodeObject(const Value& bvalue){

    if(std::holds_alternative<int64_t>(bvalue.value)){
      int64_t bint{std::get<int64_t>(bvalue.value)};
      return encode64Int(bint);
    }else if(std::holds_alternative<std::string>(bvalue.value)){
      std::string bstring{std::get<std::string>(bvalue.value)};
      return encodeString(bstring);
    }else if(std::holds_alternative<std::vector<Value>>(bvalue.value)){
      std::vector<Value> blist{std::get<std::vector<bencode::Value>>(bvalue.value)};
      return encodeList(blist);
    }else if(std::holds_alternative<std::map<std::string, Value>>(bvalue.value)){
      std::map<std::string, Value> bmap {std::get<std::map<std::string, bencode::Value>>(bvalue.value)};
      return encodeDict(bmap);
    }else{
      throw std::invalid_argument("The argument does not match any function.");
    }
  }
}
