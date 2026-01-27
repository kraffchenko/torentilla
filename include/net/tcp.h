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
  inline void write(net::Connection& connection,
                    net::CommunicationManager& com_manager);
  inline void checkHandshake(net::Connection& connection,
                             net::CommunicationManager& com_manager);
  inline void sendHandshake(Connection& connection, net::CommunicationManager& com_manager);
  inline void sendMessage(net::Connection& connection,
                          net::CommunicationManager& com_manager,
                          message::State);
  inline void sendMessage(net::Connection& connection,
                          net::CommunicationManager& com_manager,
                          message::State);
  inline void sendMessage(net::Connection& connection,
                          net::CommunicationManager& com_manager,
                          message::Have);
  inline void sendMessage(net::Connection& connection,
                          net::CommunicationManager& com_manager,
                          message::Bitfield);
  inline void sendMessage(net::Connection& connection,
                          net::CommunicationManager& com_manager,
                          message::Action);
  inline void sendMessage(net::Connection& connection,
                          net::CommunicationManager& com_manager,
                          message::Piece);
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
    if(!error){
      std::cout << "Handshake sent.";
      connection.m_handshake_sent = true;
    }else{
      std::cerr << "Error while sending handshake: " << error.what();
    }
  };
  inline void handleMessage(net::Connection& connection, message::State message, net::CommunicationManager& com_manager){
    if(message.msg_params.id == message::ID::choke){
      connection.m_peer_choking = true; 
    }else if(message.msg_params.id == message::ID::unchoke){
      connection.m_peer_choking = false;
    }else if(message.msg_params.id == message::ID::interested){
      connection.m_peer_interested = true;
    }else if(message.msg_params.id == message::ID::not_interested){
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
    if(local_bitfield.getBitfield().size() == remote_bitfield.size()){
      std::vector<size_t> missing_pieces{getMissingPieces(local_bitfield.getBitfield(), remote_bitfield)};
      if(missing_pieces.back() > local_bitfield.getPiecesAmount()){
        connection.getSocket().close();
      }else{
       for(size_t index : missing_pieces){
        com_manager.getPieceManager().addConnection(index, connection);
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
  inline void handleSend(const boost::system::error_code& error){
    if(!error){
      std::cout << "State message was written.";
    }else{
      std::cerr << "Error with writing a state message.";
    }
  }
  inline void handleRead(net::Connection& connection,
                         net::CommunicationManager& com_manager,
                         const boost::system::error_code& error, 
                         size_t bytes_transferred){
    if(!error && !connection.m_is_closed){
      if(!connection.m_handshake_checked){
        net::tcp::checkHandshake(connection, com_manager); 
      }else if(connection.m_in_buffer.size() >= 4){
        int message_length {message::getIntFromBytes(&connection.m_in_buffer[message::BytePos::LEN])};
        if(std::size(connection.m_in_buffer) > static_cast<size_t>(message_length)){
          message::ID message_id {message::getMessageID(connection.m_in_buffer)};
          std::visit([&connection, &com_manager](auto&& message) { handleMessage(connection, message, com_manager); }, 
                     message::createMessageFromBuffer(message_id, message_length, connection.m_in_buffer));
          size_t message_end {static_cast<size_t>(message_length) + message::BytePos::ID};
          connection.m_in_buffer.erase(connection.m_in_buffer.begin() + message_end);
        }
      }
      net::tcp::read(connection, com_manager);
    }else if(connection.m_is_closed){
      std::cout << "Connection was closed." << '\n';
    }else {
      std::cout << "Error while reading: " << error.what() << '\n';
    }
  };
}
namespace net::tcp{
  inline void startCommunication(net::Connection& connection,
                                 net::CommunicationManager& com_manager){
    read(connection, com_manager);
    write(connection, com_manager);
  };
  inline void read(net::Connection& connection,
                   net::CommunicationManager& com_manager) {
    connection.getSocket().async_read_some(buffer(connection.m_in_buffer),
                                           std::bind(handleRead,
                                           std::ref(connection),
                                           std::ref(com_manager),
                                           placeholders::error,
                                           placeholders::bytes_transferred)); 
  };
  inline void write(net::Connection& connection,
                    net::CommunicationManager& com_manager){
    async_write(connection.getSocket(), 
                buffer(connection.m_out_buffer),
                std::bind(handleWrite,
                          std::ref(connection),
                          std::ref(com_manager),
                          placeholders::error, 
                          placeholders::bytes_transferred));
  };
  inline void checkHandshake(net::Connection& connection,
                             net::CommunicationManager& com_manager){
    if (std::size(connection.m_in_buffer) >= 67){
      message::Handshake local_peer_handshake{.info_hash = com_manager.getFile().m_metadata.getInfoHash(),
                                              .peer_id = com_manager.getPeerId()};
      message::Handshake remote_peer_handshake{message::createHandshakeFromBuffer(connection.m_in_buffer)};
      if(local_peer_handshake == remote_peer_handshake){
        connection.m_handshake_is_valid = true;
        std::cout << "Handshake successfull.";
      }else{
        std::cerr << "Handshake failed. Closing connection to " << connection.getSocket().remote_endpoint().address();
        connection.m_handshake_is_valid = false;
        connection.m_is_closed = true;
        connection.getSocket().close();
      }
      connection.m_handshake_checked = true;
    }
  };
  inline void sendHandshake(net::Connection& connection,
                            net::CommunicationManager& com_manager){
    message::Handshake handshake{};
    handshake.info_hash = com_manager.getFile().m_metadata.getInfoHash();
    handshake.peer_id = com_manager.getPeerId();
    std::vector<std::byte> byte_array{handshake.inByteArray()}; 
    connection.m_out_buffer.insert(connection.m_out_buffer.end(), byte_array.begin(), byte_array.end());
    async_write(connection.getSocket(), buffer(handshake.inByteArray()),
                std::bind(handleHandshake,
                          std::ref(connection),
                          placeholders::error,
                          placeholders::bytes_transferred));
  };
//  inline void sendMessage(net::Connection& connection,
//                          net::CommunicationManager& com_manager,
//                          message::State message){
//    async_write(connection.getSocket(),
//                buffer(connection.createSubBuffer(message.inByteArray())),
//                std::bind(handleSend,
//                placeholders::error));
//  };
//  inline void sendMessage(net::Connection& connection,
//                          net::CommunicationManager& com_manager,
//                          message::Have message){
//    async_write(connection.getSocket(),
//                buffer(connection.createSubBuffer(message.inByteArray())),
//                std::bind(handleSend,
//                placeholders::error));
//  };
//  inline void sendMessage(net::Connection& connection,
//                          net::CommunicationManager& com_manager,
//                          message::Have message){
//    async_write(connection.getSocket(),
//                buffer(connection.createSubBuffer(message.inByteArray())),
//                std::bind(handleSend,
//                placeholders::error));
//  };
  template<typename T>
  inline void sendMessage(net::Connection& connection,
                          net::CommunicationManager& com_manager,
                          T message){
    async_write(connection.getSocket(),
                buffer(connection.createSubBuffer(message.inByteArray())),
                std::bind(handleSend,
                placeholders::error));
  };
  

}
#endif
