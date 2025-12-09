#ifndef TORRENT_H
#define TORRENT_H
#include <iostream>
#include <expected>
#include <vector>
#include <fstream>
#include "net/utils/utils.h"
#include "bencode/Encode.h"
#include "bencode/Value.h"

namespace torrent::dottorrent{
  struct Config{
    std::string file_path{};
    std::string dot_torrent_path{};
    std::string announce{};
    std::string name{};
    std::string comment{};
    int64_t piece_length{512000};
    bool is_private{};
  };
  std::expected<void, bool> createDotTorrent(Config config);
}
#endif
