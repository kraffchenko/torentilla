#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include "torrent/ResumeFile.h"
#include "torrent/dottorrent/Metadata.h"

namespace torrent{
  class File{
  public:
    std::filesystem::path m_file_path{};
    torrent::ResumeFile& m_resume_file;
    torrent::dottorrent::Metadata& m_metadata;
    void writeInFile(const std::vector<std::byte>& data,
                     const size_t index,
                     const size_t begin);
    File(std::string_view path, std::string_view file_name, torrent::ResumeFile& resume_file_ref, torrent::dottorrent::Metadata&);
  private:
    void createFile(const size_t file_size);

  };
};

#endif
