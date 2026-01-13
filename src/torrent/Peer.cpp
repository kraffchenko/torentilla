#include "torrent/Peer.h"
namespace torrent{
  Peer::Peer(std::string& ip, int port)
  : m_endpoint{net::utils::createBoostEndpoint(ip, port)}
  {
  }
  Peer::Peer(boost::asio::ip::tcp::endpoint& endpoint)
  : m_endpoint{endpoint}
  {
  }
}
