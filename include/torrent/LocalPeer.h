#ifndef LOCALPEER_H
#define LOCALPEER_H
#include "torrent/Peer.h"
#include "net/Connection.h"
#include "torrent/File.h"
#include "net/tcp.h"
#include "net/CommunicationManager.h"
#include <boost/asio.hpp>
#include <map>
#include <ranges>
namespace torrent{
class LocalPeer{
private:
  net::CommunicationManager& m_com_manager;
  boost::asio::ip::tcp::acceptor m_acceptor;
  void handleInConnection(net::Connection& connection,
                          int pending_connection_index,
                          std::array<std::byte, 20>& peer_id,
                          torrent::File& file,
                          const boost::system::error_code& error);
  void handleOutConnection(net::Connection& connection,
                           std::array<std::byte, 20>& peer_id,
                           torrent::File& file,
                           const boost::system::error_code& err);
public:
  LocalPeer(boost::asio::io_context& io_context, net::CommunicationManager& com_manager);
  void connect(torrent::Peer& remote_peer, torrent::File& file, std::array<std::byte, 20>& peer_id);
  void acceptConnection(torrent::File& file, std::array<std::byte, 20>& peer_id);
};
}
#endif
