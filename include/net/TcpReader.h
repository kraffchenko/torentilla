#ifndef TCPREADER_H
#define TCPREADER_H
#include <iostream>
#include <vector>
#include "net/Connection.h"
#include "net/TcpWriter.h"
namespace net{
  class TcpReader{
  public:
    void read(net::Connection& connection,
              const std::array<std::byte, 20>& peer_id,
              torrent::File& file);
  private:
    net::TcpWriter m_tcp_writer{};
    std::vector<std::byte> m_buffer{};
    std::vector<std::reference_wrapper<net::Connection>> m_connections{};
    void handleRead(net::Connection& connection,
                    const std::array<std::byte, 20>& peer_id, 
                    torrent::File& file,
                    const boost::system::error_code& error, 
                    size_t bytes_transferred);;
  };
}
#endif
