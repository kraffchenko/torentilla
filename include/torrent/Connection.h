#ifndef CONNECTION_H
#define CONNECTION_H
#include <iostream>
#include <array>
#include <vector>
#include <bitset>
#include <memory>
#include "boost/asio.hpp"
#include "torrent/dottorrent/Metadata.h"
#include "torrent/Message.h"
#include "net/utils/utils.h"

namespace torrent{
  class Connection{
  private:
    bool m_am_choking{true};
    bool m_am_interested{false};
    bool m_peer_choking{true};
    bool m_peer_interested{false};
    bool m_handshake_sent{false};
    bool m_handshake_checked{false};
    bool m_handshake_is_valid{false};
    std::vector<std::byte> m_out_buffer{};
    std::vector<std::byte> m_in_buffer{};
    boost::asio::ip::tcp::socket m_socket; //represent a connection by itself
    void handleMessage(torrent::message::KeepAliveMessage);
    void handleMessage(torrent::message::StateMessage);
    void handleMessage(torrent::message::HaveMessage);
    void handleMessage(torrent::message::BitfieldMessage);
    void handleMessage(torrent::message::ActionMessage);
    void handleMessage(torrent::message::PieceMessage);
    void checkHandshake(torrent::dottorrent::Metadata& metadata, 
                        std::array<std::byte, 20> peer_id);
  public:
    bool m_is_established{false};
    bool m_is_closed{false};
    Connection(boost::asio::any_io_executor io_exec);
    Connection(boost::asio::io_context& io_context);
    boost::asio::ip::tcp::socket& getSocket();
    void startCommunication(torrent::dottorrent::Metadata& metadata, 
                            std::array<std::byte, 20> peer_id);
    void read(std::array<std::byte, 20> peer_id,
              torrent::dottorrent::Metadata& metadata);
    void write(std::array<std::byte, 20> peer_id,
               torrent::dottorrent::Metadata& metadata);
    void handleWrite(std::array<std::byte, 20> peer_id, 
                     torrent::dottorrent::Metadata& metadata,
                     const boost::system::error_code& error, 
                     size_t bytes_transferred);
    void handleRead(std::array<std::byte, 20> peer_id, 
                    torrent::dottorrent::Metadata& metadata,
                    const boost::system::error_code& error, 
                    size_t bytes_transferred);
  };
}
#endif
