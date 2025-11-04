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
  
  try{
    return Metainfo{
      getValue<std::string>(general_dict, "announce"), 
      getValue<int64_t>(info_dict, "piece length"), 
      getValue<std::string>(info_dict,  "pieces"),
      getValue<std::string>(info_dict, "name"),
      getValue<int64_t>(info_dict, "length"),
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


