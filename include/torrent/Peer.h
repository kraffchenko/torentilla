#ifndef PEER_H
#define PEER_H
#include <iostream>
#include "boost/asio.hpp"
#include "net/utils/utils.h"
#include "torrent/Connection.h"
namespace torrent{
class Peer{
private:
    boost::asio::ip::tcp::endpoint m_endpoint;
public:
    std::string getIp() { return m_endpoint.address().to_string(); };
    int getPort() { return m_endpoint.port(); };
    boost::asio::ip::tcp::endpoint& getEndpoint() { return m_endpoint; }
    Peer(std::string& ip, int port);
    Peer(boost::asio::ip::tcp::endpoint& endpoint);
  };
}
#endif
