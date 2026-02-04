#include "torrent/File.h"

torrent::File::File(std::string_view path, std::string_view file_name, 
                    torrent::ResumeFile& resume_file_ref, 
                    torrent::dottorrent::Metadata& metadata)
: m_file_path{ std::filesystem::path{path} / std::filesystem::path{file_name} }, m_resume_file{resume_file_ref}, m_metadata{metadata}
{
  if(!std::filesystem::exists(m_file_path))
    createFile(m_metadata.getLength());
};

void torrent::File::createFile(const size_t file_size){
  std::ofstream file{m_file_path, std::ios::binary};
  file.seekp(file_size -1);
  file.put(0);
};
void torrent::File::writeInFile(const std::vector<std::byte>& data,
                                const size_t index, const size_t begin){
  int64_t piece_length{m_metadata.getPieceLength()};
  // piece_length * (index+1) = pos of the byte in file 
  // pos + offset = place to write
  size_t byte_pos {static_cast<size_t>(piece_length) * (index+1) + begin};
  if(byte_pos <= m_metadata.getLength()){
    std::ofstream file{m_file_path, std::ios::binary};
    file.seekp(byte_pos);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    std::cout << "torrent::File::writeInFile: data was sucessfully written in file.";
  }
  else{
    std::cout << "torrent::File::writeInFile: write is impossible - invalid byte pos.";
  }

};


