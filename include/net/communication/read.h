#ifndef READ_H
#define READ_H
#include "net/communication/write.h"
#include "net/Connection.h"
#include "torrent/File.h"
#include "net/communication/CommunicationManager.h"
#include "torrent/protocol/PieceManager.h"
#include "net/utils/utils.h"
using namespace boost::asio;
using namespace torrent::dottorrent;
using namespace torrent::protocol;
using namespace net::communication;

namespace net::communication::read{
  inline awaitable<void> read(net::Connection& connection,
                   CommunicationManager& com_manager,
                   PieceManager& piece_manager);
}
namespace{
  inline void handleMessage(net::Connection& connection, message::State message, PieceManager& piece_manager, CommunicationManager& com_manager){
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
                            PieceManager& piece_manager,
                            CommunicationManager& com_manager){
    torrent::protocol::Bitfield& bitfield{com_manager.getFile().m_resume_file.getBitfield()};
    if(bitfield.getPiecesAmount() < message.piece_index){
      com_manager.closeConnection(connection);
      return;
    }
    if(!bitfield.hasPiece(message.piece_index)){
      piece_manager.addConnection(message.piece_index, connection);
      if(!connection.m_am_interested){
        message::State interested{message::Params{message::length::STATE, message::ID::interested}};
        co_spawn(connection.getSocket().get_executor(), write::sendMessage(connection, interested), detached); 
        connection.m_am_interested = true;
      }else{
        
      }
    }
  };
  inline void handleMessage(net::Connection& connection,
                            message::Bitfield message,
                            torrent::protocol::PieceManager& piece_manager,
                            CommunicationManager& com_manager){
    Bitfield& local_bitfield{com_manager.getFile().
                             m_resume_file.getBitfield()};
    std::vector<std::byte> remote_bitfield{message.bitfield_as_byte_array};
    if(!(local_bitfield.getBitfield().size() == remote_bitfield.size())){
      com_manager.closeConnection(connection);
      return;
    }
    std::vector<size_t> missing_pieces{getMissingPieces(local_bitfield.getBitfield(), remote_bitfield)};
    if(missing_pieces.back() > local_bitfield.getPiecesAmount()){
      com_manager.closeConnection(connection);
      return;
    }
    if(missing_pieces.size() < 1){
      return;
    }
    message::State interested{message::Params{message::length::STATE, message::ID::interested}};
    co_spawn(connection.getSocket().get_executor(), write::sendMessage(connection, interested), detached); 
    connection.m_am_interested = true;
    for(size_t index : missing_pieces){
      piece_manager.addConnection(index, connection);
    } 
    connection.m_bitfield_received = true;
    std::cout << "1" << '\n';
  };
  inline void handleMessage(net::Connection& connection,
                            message::Action message,
                            torrent::protocol::PieceManager& piece_manager,
                            CommunicationManager& com_manager){
    if(message.msg_params.id == message::ID::request){
      //async_write a piece 
    }else{
      connection.getSocket().shutdown(ip::tcp::socket::shutdown_send);
    } 
  };
  inline void handleMessage(net::Connection& connection,
                            message::Piece message,
                            torrent::protocol::PieceManager& piece_manager,
                            CommunicationManager& com_manager){
    std::array<std::byte, 20> piece_hash{net::utils::returnSHA1(message.block)}; 
    auto it{com_manager.getFile().m_metadata.getPieces().begin() + 20*message.index};
    std::array<std::byte, 20> metadata_hash{};
    std::transform(it, it+19, 
                   metadata_hash.begin(),
                   [](char ch){return static_cast<std::byte>(ch); });
    if(!(piece_hash == metadata_hash)){
      com_manager.closeConnection(connection);
    }
    //check if the block was written correctly
    com_manager.getFile().writeInFile(message.block, message.index, message.begin);
    if(!piece_manager.getPiece(message.index).markBlockAsDone({message.begin, message.block.size()})){
     com_manager.closeConnection(connection); 
    }
  };
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
  inline void processPayload(net::Connection& connection, CommunicationManager& com_manager, torrent::protocol::PieceManager& piece_manager, size_t bytes_transferred){
    setupMessageBuffer(connection, bytes_transferred);
    if(connection.m_in_buffer.filled() >= connection.m_in_buffer.length()){
      message::ID message_id {message::getMessageID(connection.m_in_buffer)};
      std::cout << "processPayload: Processing message..." << '\n';
      std::visit([&connection, &com_manager, &piece_manager](auto&& message) {  handleMessage(connection, message, piece_manager, com_manager); }, 
                 message::createMessageFromBuffer(message_id, connection.m_in_buffer.length()-1, connection.m_in_buffer));
      size_t processed_bytes {connection.m_in_buffer.length()};
      connection.m_in_buffer.reset(processed_bytes);
      std::cout << "processPayload: Buffer reset." << '\n';
      if(connection.m_in_buffer.filled() > processed_bytes){
        std::cout << "processPayload: Buffer contains extra bytes, repeating processing." << '\n';
        connection.m_in_buffer.rotate(processed_bytes);
        processPayload(connection, com_manager, piece_manager, bytes_transferred - processed_bytes);
      }
    }else{
      std::cout << "processPayload: Message not complete, repeating read without reset." << '\n';
    }
  };
  inline void processHandshake(net::Connection& connection,
                               CommunicationManager& com_manager,
                               torrent::protocol::PieceManager& piece_manager,
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
    message::State choke{message::Params{message::length::STATE, message::ID::choke}};
    co_spawn(connection.getSocket().get_executor(), write::sendMessage(connection, choke), detached);
    if(bytes_transferred > message::Handshake::m_default_length){
      std::cout << "processHandshake: Buffer contains extra bytes, calling processPayload..." << '\n';
      connection.m_in_buffer.rotate(message::Handshake::m_default_length);
      processPayload(connection, com_manager, piece_manager, bytes_transferred - message::Handshake::m_default_length);
    }
  };
}
namespace net::communication::read{
  inline awaitable<void> read(net::Connection& connection,
                   CommunicationManager& com_manager,
                   torrent::protocol::PieceManager& piece_manager) {
    for(;;){
      std::cout << "reading..." << '\n'; 
      size_t bytes_transferred {co_await connection.getSocket().async_read_some(buffer(connection.m_in_buffer.getAvailableRange()), use_awaitable)};
      if(connection.m_is_closed){
        std::cout << "Connection was closed." << '\n';
        co_return;
      }
      if(!connection.m_handshake_checked){
         processHandshake(connection, com_manager, piece_manager, bytes_transferred);
      }else{
         processPayload(connection, com_manager, piece_manager, bytes_transferred);
      }
    }
  };
}
#endif
