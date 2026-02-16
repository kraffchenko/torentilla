#ifndef WRITE_H
#define WRITE_H
#include "net/Connection.h"
#include "net/communication/CommunicationManager.h"
#include <boost/asio.hpp>
using namespace boost::asio;
namespace net::communication::write{
  template<typename T>
  inline awaitable<void> sendMessage(net::Connection& connection,
                          T message){
    std::cout << "sendMessage: Moving a message into the buffer..." << '\n';
    connection.m_out_buffer.insert(message.inByteArray());
    co_await async_write(connection.getSocket(), buffer(connection.m_out_buffer.getRange((connection.m_out_buffer.length() - connection.m_out_buffer.filled()), connection.m_out_buffer.length())));
    std::cout << "handleSend: Message was written" << '\n';
    connection.m_out_buffer.reset(connection.m_out_buffer.filled());
  };
  inline awaitable<void> sendHandshake(net::Connection& connection,
                            CommunicationManager& com_manager){
    message::Handshake handshake{};
    handshake.info_hash = com_manager.getFile().m_metadata.getInfoHash();
    handshake.peer_id = com_manager.getPeerId();
    connection.m_out_buffer.insert(handshake.inByteArray());
    size_t bytes_transferred {co_await async_write(connection.getSocket(), 
                              buffer(connection.m_out_buffer.getRange(0, connection.m_out_buffer.filled())))};
    std::cout << "handleHandshake: Handshake was written." << '\n';
    connection.m_out_buffer.reset(bytes_transferred);
  };
}
#endif
