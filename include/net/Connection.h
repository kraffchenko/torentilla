#ifndef CONNECTION_H
#define CONNECTION_H
#include <iostream>
#include <array>
#include <vector>
#include <bitset>
#include <memory>
#include "torrent/protocol/Buffer.h"
#include "boost/asio.hpp"
#include "torrent/dottorrent/Metadata.h"
#include "torrent/protocol/Message.h"
#include "torrent/protocol/Piece.h"
#include "torrent/File.h"
#include "net/utils/utils.h"

using namespace torrent::protocol;
namespace net{
  class Connection{
  private:
    boost::asio::ip::tcp::socket m_socket; //represent a connection by itself
  public:
    bool m_am_choking{true};
    bool m_am_interested{false};
    bool m_peer_choking{true};
    bool m_peer_interested{false};
    bool m_handshake_sent{false};
    bool m_handshake_checked{false};
    bool m_handshake_is_valid{false};
    bool m_is_established{false};
    bool m_is_closed{false};
    bool m_bitfield_sent{false};
    bool m_bitfield_received{false};
    Buffer m_out_buffer{};
    Buffer m_in_buffer{};
    Connection(boost::asio::any_io_executor io_exec);
    Connection(boost::asio::io_context& io_context);
    boost::asio::ip::tcp::socket& getSocket();
    std::pair<std::byte*, size_t> createSubBuffer(std::vector<std::byte> message_as_ba);
  };
}
#endif
