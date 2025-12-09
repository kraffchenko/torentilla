#ifndef SESSION_H
#define SESSION_H

#include<array>
#include <iostream>
#include <random>
#include <iostream>

#include "torrent/dottorrent/file.h"
#include "torrent/dottorrent/Metadata.h"
#include "net/utils/utils.h"

class Session{
public:
  Session();
  void downloadTorrent(std::string& path);
  std::array<std::byte, 20> const getPeerID(){ return m_peer_id;};
  void createDotTorrent(torrent::dottorrent::Config config);
private:
  static std::array<std::byte, 20> generateID();
  std::array<std::byte, 20>m_peer_id {};

};


#endif
