#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H
#include <iostream>
#include "net/Connection.h"
#include "torrent/File.h"
using namespace torrent::protocol;
namespace net::communication{
  class CommunicationManager{
  public:
    torrent::File& getFile() { return m_file; };
    std::array<std::byte, 20>& getPeerId() { return m_peer_id; };
    CommunicationManager(torrent::File& file,
                         std::array<std::byte, 20>& peer_id)
    : m_file{file}, m_peer_id{peer_id} 
    {
    };
    bool connectionIsPending(net::Connection& connection);
    void addConnection(net::Connection&);
    void closeConnection(net::Connection& connection);
    net::Connection& getConnection(std::string& ip);
    bool connectionExists(std::string& ip);
  private:
    std::map<std::string, net::Connection&> m_connections{};
    torrent::File& m_file;
    std::array<std::byte, 20>& m_peer_id;
  };
}
#endif
