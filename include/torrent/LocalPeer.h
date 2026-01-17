#ifndef LOCALPEER_H
#define LOCALPEER_H
#include "torrent/Peer.h"
#include "net/Connection.h"
#include "net/TcpReader.h"
#include "torrent/File.h"
#include <boost/asio.hpp>
#include <map>
#include <ranges>
namespace torrent{
class LocalPeer{
private:
  bool m_am_choking{true};
  bool m_am_interested{false};
  net::TcpReader m_tcp_reader{};
  std::unordered_map<std::string, net::Connection> m_connections{};
  boost::asio::ip::tcp::acceptor m_acceptor;
  void handleInConnection(net::Connection connection,
                          std::array<std::byte, 20>& peer_id,
                          torrent::File& file,
                          const boost::system::error_code& error);
  void handleOutConnection(net::Connection connection,
                           std::array<std::byte, 20>& peer_id,
                           torrent::File& file,
                           const boost::system::error_code& err);
public:
  LocalPeer(boost::asio::io_context& io_context);
  void connect(torrent::Peer& remote_peer, torrent::File& file, std::array<std::byte, 20>& peer_id);
  void acceptConnection(torrent::File& file, std::array<std::byte, 20>& peer_id);
};
}
#endif
