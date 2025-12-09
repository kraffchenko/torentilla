#include "torrent/dottorrent/file.h"
namespace {  
  void addHashedPieceString(std::string& pieces, std::ifstream& file_ref, const int64_t byte_array_size){
    std::vector<std::byte> raw_bytes(byte_array_size);
    file_ref.read(reinterpret_cast<char*>(raw_bytes.data()), byte_array_size);
    std::array<std::byte, 20> chunk_sha1{net::utils::returnSHA1(raw_bytes)};
    pieces.append(reinterpret_cast<const char*>(chunk_sha1.data()), chunk_sha1.size());
  } 
  std::string createHashedPiecesString(std::ifstream& file_ref, const int64_t piece_length){
    file_ref.seekg(0, std::ios::end);
    std::streampos file_size{file_ref.tellg()};
    file_ref.seekg(0, std::ios::beg);

    std::string pieces {};
    int64_t default_chunk_size{piece_length};
    long full_chunks_amount{file_size / default_chunk_size};
    long last_chunk_size{file_size % default_chunk_size}; 

    for(size_t i{1}; i <= static_cast<size_t>(full_chunks_amount); i++){
      addHashedPieceString(pieces, file_ref, default_chunk_size);
    }
    if(last_chunk_size > 0){
      addHashedPieceString(pieces, file_ref, last_chunk_size);
    }
    return pieces;
  }
}

namespace torrent::dottorrent{
  std::expected<void, bool> createDotTorrent(Config config){
    std::ifstream file{config.file_path, std::ios::binary};
    const std::string pieces {createHashedPiecesString(file, config.piece_length)};
    std::cout << pieces << '\n';
    file.seekg(0, std::ios::end);
    const std::streampos file_size{file.tellg()};
    file.seekg(0, std::ios::beg);
    std::ofstream dot_torrent{config.dot_torrent_path + config.name, std::ios::binary};
    bencode::Encode encode{};
    std::map<std::string, bencode::Value> info_dict{
      {"piece length", bencode::Value{config.piece_length}},
      {"pieces", bencode::Value{pieces}},
      {"private", bencode::Value{config.is_private}},
      {"name", bencode::Value{config.name}},
      {"length", bencode::Value{file_size}},
    };
    std::map<std::string, bencode::Value> general_dict{
      {"info", bencode::Value{info_dict}},
      {"announce", bencode::Value{config.announce}},
      {"comment", bencode::Value{config.comment}},
    };
    const std::vector<std::byte> encoded_data{encode.encodeDict(general_dict)};
    dot_torrent.write(reinterpret_cast<const char*>(encoded_data.data()), encoded_data.size());
    return {};
  }

}
