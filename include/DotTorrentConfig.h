#ifndef DOTTORRENTCONFIG_H
#define DOTTORRENTCONFIG_H
#include <iostream>
struct DotTorrentConfig{
  std::string path{};
  std::string announce{};
  std::string name{};
  std::string comment{};
  bool is_private{};
};
#endif
