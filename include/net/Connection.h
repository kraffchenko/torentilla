#ifndef CONNECTION_H
#define CONNECTION_H
#include <iostream>
#include <array>
#include <vector>
#include <bitset>
#include <memory>
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
    inline static std::map<int32_t, Piece> pending_piece{};
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
    void handleMessage(message::KeepAlive, torrent::File&);
    void handleMessage(message::State, torrent::File& file);
    void handleMessage(message::Have, torrent::File& file);
    void handleMessage(message::Bitfield, torrent::File& file);
    void handleMessage(message::Action, torrent::File& file);
    void handleMessage(message::Piece, torrent::File& file);
    void checkHandshake(const torrent::dottorrent::Metadata& metadata, 
                        const std::array<std::byte, 20>& peer_id);
  public:
    bool m_is_established{false};
    bool m_is_closed{false};
    Connection(boost::asio::any_io_executor io_exec);
    Connection(boost::asio::io_context& io_context);
    boost::asio::ip::tcp::socket& getSocket();
    void startCommunication(const std::array<std::byte, 20>& peer_id,
                            torrent::File& File);
    void read(const std::array<std::byte, 20>& peer_id,
              torrent::File& file);
    void write(const std::array<std::byte, 20>& peer_id,
               torrent::File& file);
    void handleWrite(const std::array<std::byte, 20>& peer_id, 
                     torrent::File& file,
                     const boost::system::error_code& error, 
                     size_t bytes_transferred);
    void handleRead(const std::array<std::byte, 20>& peer_id, 
                    torrent::File& file,
                    const boost::system::error_code& error, 
                    size_t bytes_transferred);
  };
}
#endif
