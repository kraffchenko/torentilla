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
using namespace boost::asio;
namespace torrent{
  class LocalPeer{
  private:
    boost::asio::ip::tcp::acceptor m_acceptor;
  public:
    LocalPeer(boost::asio::io_context& io_context);
    awaitable<net::Connection> connect(torrent::Peer& remote_peer);
    awaitable<net::Connection> acceptConnection();
  };
}
#endif
