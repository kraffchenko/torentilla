#include "net/TcpWriter.h"
namespace net{
  void TcpWriter::write(net::Connection& connection,
                        const std::array<std::byte, 20>& peer_id,
                        torrent::File& file){
      boost::asio::async_write(connection.getSocket(), boost::asio::buffer(m_buffer),
                               std::bind(&TcpWriter::handleWrite, this,
                                         std::ref(peer_id),
                                         std::ref(connection),
                                         std::ref(file),
                                         boost::asio::placeholders::error, 
                                         boost::asio::placeholders::bytes_transferred)); 

  }
  void TcpWriter::handleWrite(const std::array<std::byte, 20>& peer_id,
                              net::Connection& connection,
                              torrent::File& file,
                              const boost::system::error_code& error, 
                              size_t bytes_transferred){


  };
}
