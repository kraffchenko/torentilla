#include "MetainfoHandler.h"

Metainfo MetainfoHandler::createMetainfo(std::string& path){
  std::ifstream file{path, std::ios::binary};
  file.seekg(0, std::ios::end);
  std::streampos file_size{file.tellg()};
  file.seekg(0, std::ios::beg);
  std::vector<std::byte> file_data(static_cast<size_t>(file_size));
  file.read( reinterpret_cast<char*>(file_data.data()), static_cast<long>(file_size) );

  BDecode decoder {file_data};
  BValue decoded_dict {decoder.parseByteArray()};

  std::map<std::string, BValue> general_dict {std::get<std::map<std::string, BValue>>(decoded_dict.value)};
  std::map<std::string, BValue> info_dict{std::get<std::map<std::string, BValue>>(general_dict.at("info").value)};

  BEncode encode{};
  std::vector<std::byte> encoded_info{encode.encodeDict(info_dict)};
  

  
  try{
    return Metainfo{
      getValue<std::string>(general_dict, "announce"), 
      getValue<int64_t>(info_dict, "piece length"), 
      getValue<std::string>(info_dict,  "pieces"),
      getValue<std::string>(info_dict, "name"),
      getValue<int64_t>(info_dict, "length"),
      getValueOpt<std::vector<std::vector<std::string>>>(general_dict, "announce-list"),
      getValueOpt<int64_t>(general_dict, "creation date"),
      getValueOpt<std::string>(general_dict, "comment"),
      getValueOpt<std::string>(general_dict, "created by"),
      getValueOpt<std::string>(general_dict, "encoding"),
      getValueOpt<int64_t>(info_dict, "private"),
      getValueOpt<std::string>(info_dict, "md5sum"),
    };
  }catch(const std::invalid_argument&  e){
    std::cerr << "MetainfoHandler::createMetaInfo: Decoded dictionary is corrupt.";
    throw std::invalid_argument(e.what());
  }

};

std::vector<std::byte> MetainfoHandler::returnRawInfo(std::vector<std::byte>& raw_bytes){
  std::byte integer_start{105}; // ASCII I 
  std::byte end_delimeter{101}; // ASCII E
  std::byte list_start{108}; // ASCII L 
  std::byte dict_start{100}; // ASCII D
  std::vector<std::byte> info_dict_signature{ std::byte{52}, std::byte{58},std::byte{105}, std::byte{110}, std::byte{102}, std::byte{111}};
  
  std::vector<std::byte> raw_info_dict{};

  auto it {std::search(raw_bytes.begin(), raw_bytes.end(), info_dict_signature.begin(), info_dict_signature.end())};
  size_t info_dict_start_index{static_cast<size_t>(std::distance(raw_bytes.begin(), it))};

  bool dict_end_delimeter_is_next{true};
  for(size_t index{info_dict_start_index}; index <= std::size(raw_bytes); index++){
    raw_info_dict.push_back(raw_bytes[index]);
    if(raw_bytes[index] == end_delimeter){
      if(dict_end_delimeter_is_next){
        std::cout << index << '\n';
        break;
      }else{
        dict_end_delimeter_is_next = true;
      }
    }else if(raw_bytes[index] == integer_start || raw_bytes[index] == list_start || raw_bytes[index] == dict_start){
      dict_end_delimeter_is_next = false;
    }
  }
  return raw_info_dict;
}

//void MetainfoHandler::parseElement(size_t pos, std::vector<std::byte>& byte_array){
  //std::byte integer_start{105}; // ASCII I 
  //std::byte end_delimeter{101}; // ASCII E
  //std::byte list_start{108}; // ASCII L 
  //std::byte dict_start{100}; //ASCII D

  //if(byte_array[pos] == integer_start){
    //addInt(pos, byte_array);
  //}else if(byte_array[pos] == list_start){
    //addList(pos, byte_array);
  //}else if(byte_array[pos] == dict_start){
    //addDict(pos, byte_array);
  //}else{
    //addString(pos, byte_array);
  //}
//}







