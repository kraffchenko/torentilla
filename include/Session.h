#ifndef SESSION_H
#define SESSION_H

#include<array>
#include <iostream>
#include <random>
#include <iostream>

#include "torrent/dottorrent/file.h"
#include "torrent/dottorrent/Metadata.h"
#include "net/utils/utils.h"
#include "torrent/Peer.h"
#include "net/Connection.h"
#include "torrent/LocalPeer.h"
#include "boost/asio.hpp"
#include "net/communication/CommunicationManager.h"
#include "tracker/Request.h"
using namespace boost::asio;
class Session{
public:
  Session();
  awaitable<void> downloadTorrent(const std::string_view dottorrent_path,
                       const std::string_view path_to_install);
  std::array<std::byte, 20> const getPeerID(){ return m_peer_id;};
  void createDotTorrent(torrent::dottorrent::Config config);
  void setConnection();
  awaitable<void> start(io_context& cntx, net::communication::CommunicationManager& com_manager, PieceManager& peer_manager);
private:
  static std::array<std::byte, 20> generateID();
  std::array<std::byte, 20>m_peer_id {};

};


#endif
