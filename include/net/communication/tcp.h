#ifndef TCP_H
#define TCP_H
#include "net/communication/read.h"
#include "net/communication/write.h"
#include "torrent/LocalPeer.h"
using namespace net;
using namespace torrent::protocol;
using namespace torrent;
namespace net::communication{
  inline awaitable<void> startCommunication(Connection connection, CommunicationManager& com_manager, PieceManager& piece_manager){
    boost::asio::ip::tcp::endpoint remote_endpoint{connection.getSocket()
                                                  .remote_endpoint()};
    std::string remote_ip{remote_endpoint.address().to_string()};
    if(com_manager.connectionExists(remote_ip)){
      std::cout << "Connection with " << remote_ip << " is closed due to redundancy";
      co_return;
    }
    com_manager.addConnection(connection);
    co_await net::communication::write::sendHandshake(connection, com_manager);
    co_await net::communication::read::read(connection, com_manager, piece_manager);
  };
};
#endif
