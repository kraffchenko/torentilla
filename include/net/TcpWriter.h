#ifndef TCPWRITER_H
#define TCPWRITER_H
#include <iostream>
#include <vector>
#include "net/Connection.h"
namespace net{
  class TcpWriter{
  public:
    void write(net::Connection& connection,
               const std::array<std::byte, 20>& peer_id,
               torrent::File& file);
  private:
    std::vector<std::byte> m_buffer{};
    std::vector<std::reference_wrapper<net::Connection>> m_connections{};
    void handleWrite(const std::array<std::byte, 20>& peer_id,
                     net::Connection& connection,
                     torrent::File& file,
                     const boost::system::error_code& error, 
                     size_t bytes_transferred);
  };
}
#endif
