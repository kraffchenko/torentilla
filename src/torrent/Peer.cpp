#include "torrent/Peer.h"
namespace torrent{
  Peer::Peer(std::string& ip, int port, boost::asio::io_context& io_context)
  : m_endpoint{net::utils::createBoostEndpoint(ip, port)}, m_connection{io_context}
  {
  }
  Peer::Peer(boost::asio::ip::tcp::endpoint& endpoint, boost::asio::io_context& io_context)
  : m_endpoint{endpoint}, m_connection{io_context}
  {
  }
  Peer::Peer(std::string& ip, int port, torrent::Connection&& connection)
  : m_endpoint{net::utils::createBoostEndpoint(ip, port)}, m_connection{std::move(connection)}
  {
  }
  Peer::Peer(boost::asio::ip::tcp::endpoint& endpoint, torrent::Connection&& connection)
  : m_endpoint{endpoint}, m_connection{std::move(connection)}
  {
  }
}
