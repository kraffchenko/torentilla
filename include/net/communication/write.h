#ifndef WRITE_H
#define WRITE_H
#include "net/Connection.h"
#include "net/communication/CommunicationManager.h"
#include <boost/asio.hpp>
using namespace boost::asio;
namespace{
  inline void handleSend(net::Connection& connection, 
                         const boost::system::error_code& error){
    if(error){
      std::cout << "handleSend: Error while writing." << '\n';
      return;
    }
    std::cout << "handleSend: Message was written" << '\n';
    connection.m_out_buffer.reset(connection.m_out_buffer.filled());
  }
  inline void handleHandshake(net::Connection& connection,
                              const boost::system::error_code& error,
                              size_t bytes_transferred){
    if(error){
      std::cout << "handleHandshake: Error while writing." << '\n';
      return;
    }
    std::cout << "handleHandshake: Handshake was written." << '\n';
    connection.m_out_buffer.reset(bytes_transferred);
  };
}
namespace net::communication::write{
  template<typename T>
  inline void sendMessage(net::Connection& connection,
                          T message){
    std::cout << "sendMessage: Moving a message into the buffer..." << '\n';
    connection.m_out_buffer.insert(message.inByteArray());
    async_write(connection.getSocket(),
                buffer(connection.m_out_buffer.getRange(0, connection.m_out_buffer.filled())),
                std::bind(handleSend,
                std::ref(connection),
                placeholders::error));

  };
  inline void sendHandshake(net::Connection& connection,
                            CommunicationManager& com_manager){
    message::Handshake handshake{};
    handshake.info_hash = com_manager.getFile().m_metadata.getInfoHash();
    handshake.peer_id = com_manager.getPeerId();
    connection.m_out_buffer.insert(handshake.inByteArray());
    async_write(connection.getSocket(), buffer(connection.m_out_buffer.getRange(0, connection.m_out_buffer.filled())),
                std::bind(handleHandshake,
                          std::ref(connection),
                          placeholders::error,
                          placeholders::bytes_transferred));
  };
}
#endif
