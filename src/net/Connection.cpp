#include "net/Connection.h"

namespace net{
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
  void Connection::startCommunication(const std::array<std::byte, 20>& peer_id,
                                      torrent::File& file){
    write(peer_id, file);
    read(peer_id, file);
  }
  void Connection::read(const std::array<std::byte, 20>& peer_id,
                        torrent::File& file){
    boost::asio::async_read(m_socket, boost::asio::buffer(m_in_buffer),
                             std::bind(&Connection::handleWrite, this,
                                       std::ref(peer_id),
                                       std::ref(file),
                                       boost::asio::placeholders::error, 
                                       boost::asio::placeholders::bytes_transferred)); 
  }
  void Connection::write(const std::array<std::byte, 20>& peer_id,
                         torrent::File& file){
    boost::asio::async_write(m_socket, boost::asio::buffer(m_out_buffer),
                             std::bind(&Connection::handleWrite, this,
                                       std::ref(peer_id),
                                       std::ref(file),
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
  void Connection::handleWrite(const std::array<std::byte, 20>& peer_id, 
                              torrent::File& file,
                              const boost::system::error_code& error, 
                              size_t bytes_transferred){
    if(!error && !m_is_closed){
      if(!m_handshake_sent){
        //sendHandshake(metadata.getInfoHash(), peer_id);
      }
    }else{
      std::cout << "Error while writing..." << '\n';
    }
  }
  void Connection::handleRead(const std::array<std::byte, 20>& peer_id,
                              torrent::File& file,
                              const boost::system::error_code& error,
                              size_t bytes_transferred){
    if(!error && !m_is_closed){
      if(!m_handshake_checked){
        checkHandshake(file.m_metadata, peer_id); 
      }else if(m_in_buffer.size() >= 4){
        int message_length {message::getIntFromBytes(&m_in_buffer[message::BytePos::LEN])};
        if(std::size(m_in_buffer) > static_cast<size_t>(message_length)){
          message::ID message_id {message::getMessageID(m_in_buffer)};
          message::Message message {message::createMessageFromBuffer(message_id, message_length, m_in_buffer)};
          size_t message_end {static_cast<size_t>(message_length) + message::BytePos::ID};
          m_in_buffer.erase(m_in_buffer.begin() + message_end);
        }
      }
      read(peer_id, file);
    }else if(m_is_closed){
      std::cout << "Connection was closed." << '\n';
    }else {
      std::cout << "Error while reading: " << error.what() << '\n';
    }
  }
  void Connection::checkHandshake(const torrent::dottorrent::Metadata& metadata, const std::array<std::byte, 20>& peer_id){
    if (std::size(m_in_buffer) >= 67){
      message::Handshake local_peer_handshake{.info_hash = metadata.getInfoHash(),
                                                       .peer_id = peer_id};
      message::Handshake remote_peer_handshake{message::createHandshakeFromBuffer(m_in_buffer)};
      if(local_peer_handshake == remote_peer_handshake){
        m_handshake_is_valid = true; 
      }else{
        m_handshake_is_valid = false;
        m_is_closed = true;
        m_socket.close();
      }
      m_handshake_checked = true;
    }
  };
  void Connection::handleMessage(message::State message, torrent::File& file){
    if(message.msg_params.id == message::ID::choke){
      m_peer_choking = true; 
    }else if(message.msg_params.id == message::ID::unchoke){
      m_peer_choking = false;
    }else if(message.msg_params.id == message::ID::interested){
      m_peer_interested = true;
    }else if(message.msg_params.id == message::ID::not_interested){
      m_peer_interested = false;
    }
  };
  void Connection::handleMessage(message::Have message, torrent::File& file){
    // check if i have the piece
    // if yes ignore if not make a request message
    // add index into pending 
    // send message
    if(!file.m_resume_file.getBitfield().hasPiece(message.piece_index)){
      
      message::Action request_message{message::Params{message::length::REQUEST, message::ID::request},
                                                      message.piece_index, 0, Piece::BLOCK_LENGTH_B};
    } 
  };
  void Connection::handleMessage(message::Bitfield message, torrent::File& file){
    //send only at the beginning 
  };
  void Connection::handleMessage(message::Action message, torrent::File&){
    if(message.msg_params.id == message::ID::request){
      //send for a piece i dont have 
    }else if(message.msg_params.id == message::ID::cancel){
      
    }
  };
  void Connection::handleMessage(message::Piece message, torrent::File&){
    // send as answer to a request if i have a piece
  };

}
