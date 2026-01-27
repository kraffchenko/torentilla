#include "torrent/ResumeFile.h"

namespace torrent{
  ResumeFile::ResumeFile(std::string_view info_hash_string, std::string_view file_name, std::string_view file_path, size_t file_size, size_t pieces_amnt)
  : m_file_name{std::string{info_hash_string} + m_default_suffix}, 
    m_root_file_name{file_name}, m_root_file_path{file_path}, 
    m_left{static_cast<int64_t>(file_size)}, m_bitfield{pieces_amnt}
  {
    writeFile(info_hash_string, m_default_suffix);
  }
  ResumeFile::ResumeFile(std::string_view info_hash_string, std::string_view file_name, 
               std::string_view file_path, int64_t downloaded,
               int64_t uploaded, int64_t left, Bitfield& bitfield)
  : m_file_name{info_hash_string}, m_root_file_name{file_name}, m_root_file_path{file_path}, m_downloaded{downloaded}, 
    m_uploaded{uploaded}, m_left{left}, m_bitfield{bitfield}  
  {
  };
  Bitfield& ResumeFile::getBitfield(){
    return m_bitfield;
  }
  void ResumeFile::writeFile(const std::string_view file_name, const std::string_view suffix){
    std::cout << "Starting to write resume file...";
    bencode::Encode encoder{};
    bencode::Decode decoder{m_bitfield.getBitfield()};
    std::map<std::string, bencode::Value> resume_map{
      {"info hash", bencode::Value{std::string{file_name}}},
      {"file name", bencode::Value{m_root_file_name}},
      {"file path", bencode::Value{m_root_file_path}},
      {"downloaded", bencode::Value{m_downloaded}},
      {"uploaded", bencode::Value{m_uploaded}},
      {"left", bencode::Value{m_left}},
      {"pieces amount", bencode::Value{static_cast<int64_t>(m_bitfield.getPiecesAmount())}},
      {"bitfield", bencode::Value{m_bitfield.toString()}}
    };
    const std::vector<std::byte> encoded_data{encoder.encodeDict(resume_map)};
    std::ofstream resume_file{m_file_path.string() + 
                              std::string{file_name} + 
                              std::string{suffix}, 
                              std::ios::binary};
    std::cout << m_file_path.string() + std::string{file_name} + std::string{suffix} << '\n';
    resume_file.write(reinterpret_cast<const char*>(encoded_data.data()), encoded_data.size());
  }
  ResumeFile ResumeFile::fromFile(const std::string_view file_path, const std::string_view file_name){
    std::cout << "Creating a resume file instance from a file...";
    std::ifstream file{std::string{file_path} + std::string{file_name}, std::ios::binary};
    file.seekg(0, std::ios::end);
    std::streampos file_size{file.tellg()};
    file.seekg(0, std::ios::beg);
    std::vector<std::byte> file_data(static_cast<size_t>(file_size));
    file.read( reinterpret_cast<char*>(file_data.data()), static_cast<long>(file_size));
    
    bencode::Decode decoder{file_data};
    bencode::Value decoded_dict {decoder.parseByteArray()};
    
    std::map<std::string, bencode::Value> resume_map {std::get<std::map<std::string, bencode::Value>>(decoded_dict.value)};
    std::string info_hash{std::get<std::string>(resume_map.at("info hash").value)};
    std::string res_file_name{std::get<std::string>(resume_map.at("file name").value)};
    std::string res_file_path{std::get<std::string>(resume_map.at("file path").value)};
    int64_t downloaded{std::get<int64_t>(resume_map.at("downloaded").value)};
    int64_t uploaded{std::get<int64_t>(resume_map.at("uploaded").value)};
    int64_t left{std::get<int64_t>(resume_map.at("left").value)};
    Bitfield bitfield {Bitfield::fromString(std::get<std::string>(resume_map.at("bitfield").value),
                        static_cast<size_t>(std::get<int64_t>(resume_map.at("pieces amount").value)))};
    return ResumeFile{info_hash, res_file_name, res_file_path, downloaded, uploaded, left, bitfield};
  }
}
