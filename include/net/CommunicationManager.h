#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H
#include <iostream>
#include "torrent/protocol/PieceManager.h"
#include "torrent/File.h"
using namespace torrent::protocol;
namespace net{
  class CommunicationManager{
  public:
    torrent::File& getFile() { return m_file; };
    std::array<std::byte, 20>& getPeerId() { return m_peer_id; };
    PieceManager& getPieceManager() { return m_piece_manager; };
    CommunicationManager(torrent::File& file,
                         std::array<std::byte, 20>& peer_id)
    : m_file{file}, m_peer_id{peer_id}, 
      m_piece_manager{static_cast<size_t>(file.m_metadata.getLength()),
                      static_cast<size_t>(file.m_metadata.getPieceLength())}
    {
    };
    bool connectionIsPending(net::Connection& connection);
    void addConnection(net::Connection&&);
    void addConnection(std::string& ip, net::Connection&&);
    void closePendingConnection(int index);
    void closeConnection(net::Connection& connection);
    int addPendingConnection(net::Connection&&);
    net::Connection& getConnection(std::string& ip);
    net::Connection& getPendingConnection(int index);
    bool connectionExists(std::string& ip);
  private:
    std::map<std::string, net::Connection> m_connections{};
    std::map<int, net::Connection> m_pending_connections{};
    torrent::File& m_file;
    std::array<std::byte, 20>& m_peer_id;
    PieceManager m_piece_manager;
  };
}
#endif
