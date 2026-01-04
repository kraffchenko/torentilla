#ifndef PEER_H
#define PEER_H
#include <iostream>
#include "boost/asio.hpp"
#include "net/utils/utils.h"
#include "torrent/Connection.h"
namespace torrent{
  class Peer{
  private:
    bool m_am_choking{true};
    bool m_am_interested{false};
    boost::asio::ip::tcp::endpoint m_endpoint;
    torrent::Connection m_connection;
public:
    std::string getIp() { return m_endpoint.address().to_string(); };
    int getPort() { return m_endpoint.port(); };
    boost::asio::ip::tcp::endpoint& getEndpoint() { return m_endpoint; }
    torrent::Connection& getConnection() { return m_connection; };
    Peer(std::string& ip, int port, boost::asio::io_context& io_context);
    Peer(boost::asio::ip::tcp::endpoint& endpoint, boost::asio::io_context& io_context);
    Peer(std::string& ip, int port, torrent::Connection&& connection);
    Peer(boost::asio::ip::tcp::endpoint& endpoint, torrent::Connection&& connection);
  };
}
#endif
