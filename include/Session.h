#ifndef SESSION_H
#define SESSION_H

#include<array>
#include <iostream>
#include <random>
#include <array>
#include <iostream>

#include "torrent/Download.h"
#include "net/Request.h"
#include "metainfo/Handler.h"
#include "metainfo/Data.h"

class Session{
public:
  Session();
  void downloadTorrent(std::string& path);
  std::array<std::byte, 20> const getPeerID(){ return m_peer_id;};
private:
  static std::array<std::byte, 20> generateID();
  std::array<std::byte, 20>m_peer_id {};

};


#endif
