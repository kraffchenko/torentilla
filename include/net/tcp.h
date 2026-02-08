#ifndef TCP_H
#define TCP_H
#include "net/Connection.h"
#include "torrent/File.h"
#include "net/CommunicationManager.h"
#include "net/utils/utils.h"
using namespace boost::asio;
using namespace torrent::dottorrent;
using namespace torrent::protocol;
namespace net::tcp{
  inline void startCommunication();
  inline void read(net::Connection& connection,
                   net::CommunicationManager& com_manager);
  inline void sendHandshake(Connection& connection, net::CommunicationManager& com_manager);
}
namespace{
  inline void handleWrite(net::Connection& connection,
                          net::CommunicationManager& com_manager,
                          const boost::system::error_code& error, 
                          size_t bytes_transferred){
    if(!error){
      if(!connection.m_bitfield_sent){
        
      }
    }else{

    }
  };
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
  inline void handleMessage(net::Connection& connection, message::State message, net::CommunicationManager& com_manager){
    if(message.msg_params.id == message::ID::choke){
      std::cout << "Choke sent." << '\n';
      connection.m_peer_choking = true; 
    }else if(message.msg_params.id == message::ID::unchoke){
      std::cout << "Unchoke sent." << '\n';
      connection.m_peer_choking = false;
    }else if(message.msg_params.id == message::ID::interested){
      std::cout << "Interested sent." << '\n';
      connection.m_peer_interested = true;
    }else if(message.msg_params.id == message::ID::not_interested){
      std::cout << "Not interested sent." << '\n';
      connection.m_peer_interested = false;
    }
  };
  inline void handleMessage(net::Connection& connection, 
                            message::Have message, 
                            net::CommunicationManager& com_manager){
    torrent::protocol::Bitfield& bitfield{com_manager.getFile().m_resume_file.getBitfield()};
    if(bitfield.getPiecesAmount() <= message.piece_index){
      if(!bitfield.hasPiece(message.piece_index)){
          com_manager.getPieceManager().addConnection(message.piece_index, connection);
          //send one block and handle the rest in proactive write function
       }
    }
  };
  inline void handleMessage(net::Connection& connection,
                            message::Bitfield message,
                            net::CommunicationManager& com_manager){
    Bitfield& local_bitfield{com_manager.getFile().
                             m_resume_file.getBitfield()};
    std::vector<std::byte> remote_bitfield{message.bitfield_as_byte_array};
    std::cout << local_bitfield.getBitfield().size() << "btf" << '\n';
    if(local_bitfield.getBitfield().size() == remote_bitfield.size()){
      std::vector<size_t> missing_pieces{getMissingPieces(local_bitfield.getBitfield(), remote_bitfield)};
      if(missing_pieces.back() > local_bitfield.getPiecesAmount()){
        connection.getSocket().close();
      }else{
       for(size_t index : missing_pieces){
        com_manager.getPieceManager().addConnection(index, connection);
        //std::cout << "Remote Peer has piece: " << index << '\n';
        // send a request for a random piece
        } 
      }
    }else{
      connection.getSocket().close();
    }
  };
  inline void handleMessage(net::Connection& connection,
                            message::Action message,
                            net::CommunicationManager& com_manager){
    if(message.msg_params.id == message::ID::request){
      //async_write a piece 
    }else{
      connection.getSocket().shutdown(ip::tcp::socket::shutdown_send);
    } 
  };
  inline void handleMessage(net::Connection& connection,
                            message::Piece message,
                            net::CommunicationManager& com_manager){
    std::array<std::byte, 20> piece_hash{net::utils::returnSHA1(message.block)}; 
    auto it{com_manager.getFile().m_metadata.getPieces().begin() + 20*message.index};
    std::array<std::byte, 20> metadata_hash{};
    std::transform(it, it+19, 
                   metadata_hash.begin(),
                   [](char ch){return static_cast<std::byte>(ch); });
    if(piece_hash == metadata_hash){
      com_manager.getFile().writeInFile(message.block, message.index, message.begin);
    }else{
      connection.getSocket().close();
      //add a connection to a ban list
    }
  };
  inline void handleSend(net::Connection& connection, net::CommunicationManager& com_manager, 
                         const boost::system::error_code& error){
    if(error){
      std::cout << "handleSend: Error while writing." << '\n';
      return;
    }
    std::cout << "handleSend: Message was written" << '\n';
    connection.m_out_buffer.reset(connection.m_out_buffer.filled());
  }
  inline void setupMessageBuffer(net::Connection& connection, size_t bytes_transferred){
    connection.m_in_buffer.setFilled(bytes_transferred);
    std::cout << "setupMessageBuffer: " << bytes_transferred << " transported." << '\n';
    if(connection.m_in_buffer.filled() >= 4 && !connection.m_in_buffer.isProcessingMessage()){
      connection.m_in_buffer.setLength(message::getIntFromBytes(&connection.m_in_buffer[message::BytePos::LEN]));
      std::cout << "setupMessageBuffer: Length set to " << connection.m_in_buffer.length() << " bytes." << '\n';
      if(connection.m_in_buffer.size() < connection.m_in_buffer.length()){
        connection.m_in_buffer.resize(connection.m_in_buffer.length());
        std::cout << "setupMessageBuffer: Buffer is being resized to " << connection.m_in_buffer.length() << " bytes." << '\n';
      }
    }
  };
  inline void setupHandshakeBuffer(net::Connection& connection, size_t bytes_transferred){
    connection.m_in_buffer.setFilled(bytes_transferred);
    if(!connection.m_in_buffer.isProcessingMessage()){
      std::cout << "setupHandshakeBuffer: " << bytes_transferred << " transported." << '\n';
      connection.m_in_buffer.setLength(message::Handshake::m_default_length);
      std::cout << "setupHandshakeBuffer: Length set to " << connection.m_in_buffer.length() << " bytes." << '\n';
    }
  };
  inline bool handshakeIsSame(net::Connection& connection, message::Handshake& local_handshake, message::Handshake& remote_handshake){
    connection.m_handshake_checked = true;
    if(local_handshake == remote_handshake){
      connection.m_handshake_is_valid = true;
      std::cout << "processHandshake: Handshake successfull." << '\n';
      return true;
    }else{
      std::cerr << "processHandshake: Handshake failed. Closing connection to " << connection.getSocket().remote_endpoint().address() << '\n';
      connection.m_handshake_is_valid = false;
      connection.m_is_closed = true;
      connection.getSocket().close();
      return false;
    }
  }
  inline void processPayload(net::Connection& connection, net::CommunicationManager& com_manager, size_t bytes_transferred){
    setupMessageBuffer(connection, bytes_transferred);
    if(connection.m_in_buffer.filled() >= connection.m_in_buffer.length()){
      message::ID message_id {message::getMessageID(connection.m_in_buffer)};
      std::cout << "processPayload: Processing message..." << '\n';
      std::visit([&connection, &com_manager](auto&& message) { handleMessage(connection, message, com_manager); }, 
                 message::createMessageFromBuffer(message_id, connection.m_in_buffer.length()-1, connection.m_in_buffer));
      size_t processed_bytes {connection.m_in_buffer.length()};
      connection.m_in_buffer.reset(processed_bytes);
      std::cout << "processPayload: Buffer reset." << '\n';
      if(connection.m_in_buffer.filled() > processed_bytes){
        std::cout << "processPayload: Buffer contains extra bytes, repeating processing." << '\n';
        connection.m_in_buffer.rotate(processed_bytes);
        processPayload(connection, com_manager, bytes_transferred - processed_bytes);
      }
    }else{
      std::cout << "processPayload: Message not complete, repeating read without reset." << '\n';
    }
  };
  inline void processHandshake(net::Connection& connection,
                               net::CommunicationManager& com_manager,
                               size_t bytes_transferred){
    setupHandshakeBuffer(connection, bytes_transferred);
    if(!(connection.m_in_buffer.filled() >= message::Handshake::m_default_length)){
      std::cout << "processHandshake: Handshake not complete, repeating read without reset." << '\n';
      connection.m_in_buffer.processing(true); 
      return;
    }
    message::Handshake local_handshake{.info_hash = com_manager.getFile().m_metadata.getInfoHash(),
                                       .peer_id = com_manager.getPeerId()};
    message::Handshake remote_handshake{message::createHandshakeFromBuffer(connection.m_in_buffer)};
    if(!handshakeIsSame(connection, local_handshake, remote_handshake)){
     return; 
    }
    connection.m_in_buffer.reset(connection.m_in_buffer.filled());
    if(bytes_transferred > message::Handshake::m_default_length){
      std::cout << "processHandshake: Buffer contains extra bytes, calling processPayload..." << '\n';
      connection.m_in_buffer.rotate(message::Handshake::m_default_length);
      processPayload(connection, com_manager, bytes_transferred - message::Handshake::m_default_length);
    }
  };
  inline void handleRead(net::Connection& connection,
                         net::CommunicationManager& com_manager,
                         const boost::system::error_code& error, 
                         size_t bytes_transferred){
    if(connection.m_is_closed){
      std::cout << "Connection was closed." << '\n';
      return;
    }else if(error){
      std::cout << "Error while reading: " << error.what() << '\n';
      return;
    }
    if(!connection.m_handshake_checked){
      processHandshake(connection, com_manager, bytes_transferred);
    }else{
      processPayload(connection, com_manager, bytes_transferred);
    }
    net::tcp::read(connection, com_manager);
  };
}

namespace net::tcp{
  inline void startCommunication(net::Connection& connection,
                                 net::CommunicationManager& com_manager){
    read(connection, com_manager);
  };
  inline void read(net::Connection& connection,
                   net::CommunicationManager& com_manager) {
    std::cout << "reading..." << '\n'; 
    connection.getSocket().async_read_some(buffer(connection.m_in_buffer.getAvailableRange()),
                                           std::bind(handleRead,
                                           std::ref(connection),
                                           std::ref(com_manager),
                                           placeholders::error,
                                           placeholders::bytes_transferred)); 
  };
  inline void sendHandshake(net::Connection& connection,
                            net::CommunicationManager& com_manager){
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
  template<typename T>
  inline void sendMessage(net::Connection& connection,
                          net::CommunicationManager& com_manager,
                          T message){
    std::cout << "sendMessage: Moving a message into the buffer..." << '\n';
    connection.m_out_buffer.insert(message.inByteArray());
    async_write(connection.getSocket(),
                buffer(connection.m_out_buffer.getRange(0, connection.m_out_buffer.filled())),
                std::bind(handleSend,
                std::ref(connection),
                std::ref(com_manager),
                placeholders::error));
  };
  

}
#endif
