#ifndef LOCALPEER_H
#define LOCALPEER_H
#include "torrent/Peer.h"
#include "torrent/Connection.h"
#include <boost/asio.hpp>
#include <map>
#include <ranges>
namespace torrent{
class LocalPeer{
private:
  bool m_am_choking{true};
  bool m_am_interested{false};
  std::unordered_map<std::string, torrent::Peer> m_connections{};
  boost::asio::ip::tcp::acceptor m_acceptor;
  void handleInConnection(torrent::Connection connection_pointer,
                        const boost::system::error_code& error);
  void handleOutConnection(torrent::Peer& remote_peer, 
                           const boost::system::error_code& err);
public:
  LocalPeer(boost::asio::io_context& io_context);
  void connect(torrent::Peer& remote_peer);
  void acceptConnection();
};
}
#endif
