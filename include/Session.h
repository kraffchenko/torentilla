#ifndef SESSION_H
#define SESSION_H

#include<array>
#include <iostream>
#include <random>
#include <iostream>

#include "torrent/torrent.h"
#include "net/utils/utils.h"
#include "metainfo/Data.h"

class Session{
public:
  Session();
  void downloadTorrent(std::string& path);
  std::array<std::byte, 20> const getPeerID(){ return m_peer_id;};
  void createDotTorrent(torrent::Config config);
private:
  static std::array<std::byte, 20> generateID();
  std::array<std::byte, 20>m_peer_id {};

};


#endif
