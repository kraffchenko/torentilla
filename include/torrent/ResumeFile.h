#ifndef RESUMEFILE_H
#define RESUMEFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include <array>
#include<map>

#include "bencode/Decode.h"
#include "bencode/Encode.h"
#include "torrent/protocol/Bitfield.h"

using namespace torrent::protocol;
namespace torrent{
  class ResumeFile{
  public:
    static constexpr std::string m_default_suffix{".res"};
    static constexpr std::string m_temp_suffix{".res.tmp"};
    int64_t getUploaded();
    int64_t getDownloaded();
    int64_t getLeft();
    Bitfield& getBitfield();
    void setUploaded(const int64_t bytes);
    void setDownloaded(const int64_t bytes);
    void setLeft(const int64_t bytes);
    ResumeFile(std::string_view info_hash_string, std::string_view file_name, std::string_view file_path, size_t file_size, size_t piece_amnt);
    static ResumeFile fromFile(const std::string_view file_path, const std::string_view file_name);
    void writeFile(const std::string_view file_name, const std::string_view suffix);
  private:
    ResumeFile(std::string_view info_hash_string, std::string_view file_name, 
               std::string_view file_path, int64_t downloaded,
               int64_t uploaded, int64_t left, Bitfield& bitfield);
    std::filesystem::path home_dir {std::getenv("HOME")};
    std::filesystem::path m_file_path{home_dir 
                                      / ".config"
                                      / "torrentilla"
                                      / "resume/"};
    std::string m_file_name{};
    std::string m_root_file_path{};
    std::string m_root_file_name{};
    int64_t m_downloaded{0};
    int64_t m_uploaded{0};
    int64_t m_left{};
    Bitfield m_bitfield;
  };
}

#endif
