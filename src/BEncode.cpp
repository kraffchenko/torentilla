#include "BEncode.h"

std::vector<std::byte> BEncode::encode64Int(const int64_t& bint){
    std::vector<std::byte> bint_byte_array{};
    std::string integer_representation {"i"};
    integer_representation += std::to_string(bint) += "e";
    for(char bchar : integer_representation){
       bint_byte_array.push_back(static_cast<std::byte>(bchar)); 
    }
    return bint_byte_array;
};

std::vector<std::byte> BEncode::encodeString(const std::string& bstring){
    std::vector<std::byte> bstring_byte_array{};
    const size_t string_length{bstring.length()};
    std::string encoded_bstring {std::to_string(string_length) + ":" + bstring};
    for (char bchar : encoded_bstring){
      bstring_byte_array.push_back(static_cast<std::byte>(bchar));
    }
    return bstring_byte_array;
};

std::vector<std::byte> BEncode::encodeList(const std::vector<BValue>& blist){
    std::vector<std::byte> blist_byte_array{};
    for (BValue bvalue : blist){
      std::vector<std::byte> bvalue_byte_array= encodeObject(bvalue);
      blist_byte_array.insert(blist_byte_array.end(), bvalue_byte_array.begin(), bvalue_byte_array.end());
    }
    return blist_byte_array;
};

std::vector<std::byte> BEncode::encodeDict(const std::map<std::string, BValue>& bmap){
  std::vector<std::byte> bmap_byte_array;
  std::map<std::string, BValue>::const_iterator it;
  for(it = bmap.begin(); it != bmap.end(); it++){
    std::vector<std::byte> bstring_byte_vector {encodeString(it->first)};
    std::vector<std::byte> bvalue_byte_vector {encodeObject(it->second)};
    bmap_byte_array.insert(bmap_byte_array.end(), bstring_byte_vector.begin(), bstring_byte_vector.end());
    bmap_byte_array.insert(bmap_byte_array.end(), bvalue_byte_vector.begin(), bvalue_byte_vector.end());
  }
  return bmap_byte_array;

}

std::vector<std::byte> BEncode::encodeObject(const BValue& bvalue){

  if(std::holds_alternative<int64_t>(bvalue.value)){
    int64_t bint{std::get<int64_t>(bvalue.value)};
    return encode64Int(bint);
  }else if(std::holds_alternative<std::string>(bvalue.value)){
    std::string bstring{std::get<std::string>(bvalue.value)};
    return encodeString(bstring);
  }else if(std::holds_alternative<std::vector<BValue>>(bvalue.value)){
    std::vector<BValue> blist{std::get<std::vector<BValue>>(bvalue.value)};
    return encodeList(blist);
  }else if(std::holds_alternative<std::map<std::string, BValue>>(bvalue.value)){
    std::map<std::string, BValue> bmap {std::get<std::map<std::string, BValue>>(bvalue.value)};
    return encodeDict(bmap);
  }else{
    throw std::invalid_argument("The argument does not match any function.");
  }



}
