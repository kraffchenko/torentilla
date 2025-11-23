#ifndef SESSION_H
#define SESSION_H

#include<array>
#include <iostream>
#include <random>
#include <array>
#include <iostream>

#include "TorrentDownload.h"
#include "TrackerRequest.h"
#include "MetainfoHandler.h"
#include "Metainfo.h"

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
