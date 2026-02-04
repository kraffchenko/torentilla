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
    static inline std::filesystem::path m_home {std::getenv("HOME")};
    static inline std::filesystem::path m_default_directory {m_home 
                                                             / ".config"
                                                             / "torrentilla"
                                                             / "resume/"};
    enum class WriteLocation{
      temp,
      def
    };
    static inline std::filesystem::path pathBasedOnHash(std::string_view info_hash){
      return m_default_directory / (std::string{info_hash} + ".res");
    };

    int64_t getUploaded();
    int64_t getDownloaded();
    int64_t getLeft();
    Bitfield& getBitfield();
    std::filesystem::path& getWriteLocation(WriteLocation write_location);
    void setUploaded(const int64_t bytes);
    void setDownloaded(const int64_t bytes);
    void setLeft(const int64_t bytes);
    ResumeFile(std::string_view info_hash_string, std::string_view file_name, std::string_view file_path, size_t file_size, size_t piece_amnt);
    static ResumeFile fromFile(std::filesystem::path path_to_res_file);
    void writeFile(WriteLocation write_location);
  private:
    ResumeFile(std::string_view info_hash_string, std::string_view file_name, 
               std::string_view file_path, int64_t downloaded,
               int64_t uploaded, int64_t left, Bitfield& bitfield);
    std::filesystem::path m_temp_path{};
    std::filesystem::path m_def_path{};
    std::filesystem::path m_ref_torrent_path{};
    int64_t m_downloaded{0};
    int64_t m_uploaded{0};
    int64_t m_left{};
    Bitfield m_bitfield;
  };
}

#endif
