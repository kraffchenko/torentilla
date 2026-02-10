#ifndef LOCALPEER_H
#define LOCALPEER_H
#include "torrent/Peer.h"
#include "net/Connection.h"
#include "torrent/File.h"
#include "net/communication/write.h"
#include "net/communication/read.h"
#include "net/communication/CommunicationManager.h"
#include "torrent/protocol/PieceManager.h"
#include <boost/asio.hpp>
#include <map>
#include <ranges>
using namespace net::communication;
namespace torrent{
class LocalPeer{
private:
  CommunicationManager& m_com_manager;
  torrent::protocol::PieceManager& m_piece_manager;
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
  LocalPeer(boost::asio::io_context& io_context, 
            CommunicationManager& com_manager, 
            torrent::protocol::PieceManager& piece_manager);
  void connect(torrent::Peer& remote_peer, torrent::File& file, std::array<std::byte, 20>& peer_id);
  void acceptConnection(torrent::File& file, std::array<std::byte, 20>& peer_id);
};
}
#endif
