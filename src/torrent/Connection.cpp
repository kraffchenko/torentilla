#include "torrent/Connection.h"

namespace torrent{
   //void Connection::sendHandshake(std::array<std::byte, 20> info_hash, std::array<std::byte, 20> peer_id){
   //m_out_buffer.clear();

   // int pstrlen{19};
   // std::string pstr{"BitTorrent protocol"};
   // std::array<std::byte, 8> reserved;
   // reserved.fill(static_cast<std::byte>(0));

   // m_out_buffer.push_back(static_cast<std::byte>(pstrlen));
   // auto it_pstr {std::transform(pstr.begin(), pstr.end(), &m_out_buffer.back(), [](char ch){ return static_cast<std::byte>(ch); })};
   // auto it_reserved {std::copy(reserved.begin(), reserved.end(), it_pstr)};
   // auto it_info{std::copy(info_hash.begin(), info_hash.end(), it_reserved)};
   // std::copy(peer_id.begin(), peer_id.end(), it_info);
   // }
  void Connection::startCommunication(torrent::dottorrent::Metadata& metadata,
                                      std::array<std::byte, 20> peer_id){
    write(peer_id, metadata);
    read(peer_id, metadata);
  }
  void Connection::read(std::array<std::byte, 20> peer_id,
                         torrent::dottorrent::Metadata& metadata){
    boost::asio::async_read(m_socket, boost::asio::buffer(m_in_buffer),
                             std::bind(&Connection::handleWrite, this,
                                       peer_id,
                                       std::ref(metadata),
                                       boost::asio::placeholders::error, 
                                       boost::asio::placeholders::bytes_transferred)); 
  }
  void Connection::write(std::array<std::byte, 20> peer_id,
                         torrent::dottorrent::Metadata& metadata){
    boost::asio::async_write(m_socket, boost::asio::buffer(m_out_buffer),
                             std::bind(&Connection::handleWrite, this,
                                       peer_id,
                                       std::ref(metadata),
                                       boost::asio::placeholders::error, 
                                       boost::asio::placeholders::bytes_transferred)); 
  }
  Connection::Connection(boost::asio::io_context& io_context)
  : m_socket{io_context.get_executor()}
  {
  }
  Connection::Connection(boost::asio::any_io_executor io_exec)
  : m_socket {io_exec}
  {
  }
  boost::asio::ip::tcp::socket& Connection::getSocket(){
    return m_socket;
  }
  void Connection::handleWrite(std::array<std::byte, 20> peer_id, 
                              torrent::dottorrent::Metadata& metadata,
                              const boost::system::error_code& error, 
                              size_t bytes_transferred){
    if(!error){
      if(!m_handshake_sent){
        //sendHandshake(metadata.getInfoHash(), peer_id);
      }
    }else{
      std::cout << "Error while writing..." << '\n';
    }
  }
  void Connection::handleRead(std::array<std::byte, 20> peer_id,
                              torrent::dottorrent::Metadata& metadata,
                              const boost::system::error_code& error,
                              size_t bytes_transferred){
    if(!error){
      if(!m_handshake_checked){
        checkHandshake(metadata, peer_id); 
      }else if(m_in_buffer.size() >= 4){
        int message_length {torrent::message::getIntFromBytes(&m_in_buffer[torrent::message::BytePos::LEN])};
          if(std::size(m_in_buffer) > static_cast<size_t>(message_length)){
          torrent::message::MessageID message_id {torrent::message::getMessageID(m_in_buffer)};
          torrent::message::Message message {torrent::message::createMessageFromBuffer(message_id, message_length, m_in_buffer)};
          size_t message_end {static_cast<size_t>(message_length) + torrent::message::BytePos::ID};
          m_in_buffer.erase(m_in_buffer.begin() + message_end);
        }
      }else{
          read(peer_id, metadata);
      } 
    }else{
      std::cout << "Error while reading..." << '\n';
    }
  }
  bool Connection::checkHandshake(torrent::dottorrent::Metadata& metadata, std::array<std::byte, 20> peer_id){
    if (std::size(m_in_buffer) >= 67){
      torrent::message::Handshake local_peer_handshake{.info_hash = metadata.getInfoHash(),
                                                       .peer_id = peer_id};
      torrent::message::Handshake remote_peer_handshake{torrent::message::createHandshakeFromBuffer(m_in_buffer)};
      if(local_peer_handshake == remote_peer_handshake){
        m_handshake_is_valid = true; 
      }else{
        m_handshake_is_valid = false;
        m_socket.close();
      }
      m_handshake_checked = true;
    }
  };
  void Connection::setProperties(torrent::message::Message message){
    if(std::holds_alternative<torrent::message::StateMessage>(message)){
      
    }else if(std::holds_alternative<torrent::message::HaveMessage>(message)){
      
    }else if(std::holds_alternative<torrent::message::BitfieldMessage>(message)){
        
    }else if(std::holds_alternative<torrent::message::ActionMessage>(message)){
    
    }else if(std::holds_alternative<torrent::message::PieceMessage>(message)){
      
    }
  }
}
