#include "MetainfoHandler.h"
#include "TrackerRequest.h"
#include "Session.h"

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
      returnInfoValueHash(info_dict),
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


std::array<std::byte, 20> MetainfoHandler::returnInfoValueHash(const std::map<std::string, BValue>& info_dict_ref){
  BEncode encoder{};
  std::vector<std::byte> raw_bytes {encoder.encodeDict(info_dict_ref)};

  if (std::size(raw_bytes) <= 0){
    throw std::invalid_argument("Invalid argument");
  }
  std::array<std::byte, 20> info_hash{};

  EVP_MD_CTX* mdctx{EVP_MD_CTX_create()}; //creates a pointer to the type EVP_MD_CTX(digest context type) by using EVP_MD_CTX_create() which allocates, initializes and returns digest context
  const EVP_MD *md {EVP_sha1()}; //initializes a variable which holds md(message digest=algorith to use) 
  unsigned int md_len {};
  EVP_DigestInit_ex(mdctx, md, NULL); //setting up the digest context with supplying it with necessary values/variables
  EVP_DigestUpdate(mdctx, raw_bytes.data(), std::size(raw_bytes)); //hashes the value of raw_bytes into digest context
  EVP_DigestFinal_ex(mdctx, reinterpret_cast<unsigned char*>(info_hash.data()), &md_len); //places the value of the digest context from the previous step and saves it into info_hash 
  EVP_MD_CTX_destroy(mdctx);

  
  return info_hash;
}






