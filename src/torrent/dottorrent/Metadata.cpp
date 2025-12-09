#include "torrent/dottorrent/Metadata.h"
#include "net/utils/utils.h"
namespace torrent::dottorrent{
  Metadata::Metadata(std::string_view announce, int64_t piece_length, std::string_view pieces, std::string_view name,
          int64_t length, std::array<std::byte, 20> info_hash, std::optional<std::vector<std::vector<std::string>>> announce_list=std::nullopt, std::optional<int64_t>  creation_date=std::nullopt, 
          std::optional<std::string_view> comment=std::nullopt, 
          std::optional<std::string_view> created_by=std::nullopt, std::optional<std::string_view> encoding=std::nullopt,
          std::optional<int> is_private=std::nullopt, std::optional<std::string_view> md5sum=std::nullopt)
          : m_announce{announce}, m_piece_length {piece_length}, m_pieces{pieces}, m_name{name}, m_length{length}, m_info_hash{info_hash}, m_announce_list{announce_list}, m_creation_date{creation_date},
            m_comment{comment}, m_created_by{created_by}, m_encoding{encoding}, m_is_private{is_private}, m_md5sum{md5sum}
          {
          };

  Metadata fromDotTorrent(std::string& path){
    std::ifstream file{path, std::ios::binary};
    file.seekg(0, std::ios::end);
    std::streampos file_size{file.tellg()};
    file.seekg(0, std::ios::beg);
    std::vector<std::byte> file_data(static_cast<size_t>(file_size));
    file.read( reinterpret_cast<char*>(file_data.data()), static_cast<long>(file_size));

    bencode::Decode decoder {file_data};
    bencode::Value decoded_dict {decoder.parseByteArray()};

    const std::map<std::string, bencode::Value> general_dict {std::get<std::map<std::string, bencode::Value>>(decoded_dict.value)};
    const std::map<std::string, bencode::Value> info_dict{std::get<std::map<std::string, bencode::Value>>(general_dict.at("info").value)}; 
    
    bencode::Encode encoder{};
    const std::vector<std::byte> info_dict_raw {encoder.encodeDict(info_dict)};
    
    try{
      return Metadata{
        getValue<std::string>(general_dict, "announce"), 
        getValue<int64_t>(info_dict, "piece length"), 
        getValue<std::string>(info_dict, "pieces"),
        getValue<std::string>(info_dict, "name"),
        getValue<int64_t>(info_dict, "length"),
        net::utils::returnSHA1(info_dict_raw),
        getValueOpt<std::vector<std::vector<std::string>>>(general_dict, "announce-list"),
        getValueOpt<int64_t>(general_dict, "creation date"),
        getValueOpt<std::string>(general_dict, "comment"),
        getValueOpt<std::string>(general_dict, "created by"),
        getValueOpt<std::string>(general_dict, "encoding"),
        getValueOpt<int64_t>(info_dict, "private"),
        getValueOpt<std::string>(info_dict, "md5sum"),
      };
    }catch(const std::invalid_argument&  e){
      std::cerr << "metainfo::fromDotTorrent: Decoded dictionary is corrupt.";
      throw std::invalid_argument(e.what());
    }
  };
}
