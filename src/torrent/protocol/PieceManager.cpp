#include "torrent/protocol/PieceManager.h"
namespace torrent::protocol{
  PieceManager::PieceManager(size_t file_size, size_t piece_size)
  : m_range{0, m_all_pieces.size()}
  {
    initializePieces(file_size, piece_size);
    m_connections.resize(m_all_pieces.size());
  };
  bool PieceManager::areConnectionsWithPiece(size_t index){
    if(!m_connections.at(index).empty()){
      return true;
    }else{
      return false;
    }
  };
  bool PieceManager::allPiecesAreCompleted(){
    return m_completed_pieces == m_all_pieces.size();
  };
  std::vector<std::reference_wrapper<net::Connection>>& PieceManager::getConnectionsByIndex(int index){
   return m_connections.at(index); 
  };
  void PieceManager::addConnection(size_t index, net::Connection& connection){
    m_connections.at(index).push_back(connection);
  };
  Piece& PieceManager::getPiece(size_t index){
    return m_all_pieces[index];
  };
  size_t PieceManager::getRandomPieceIndex(){
    return m_range(m_mt);
  }
  size_t PieceManager::getPiecesAmount(){
    return m_all_pieces.size();
  }
  bool PieceManager::pieceIsReadyToDownload(size_t index){
    return (!m_all_pieces[index].isCompleted() && m_all_pieces[index].hasBlockToDownload()
            && areConnectionsWithPiece(index));
  }
  void PieceManager::initializePieces(size_t file_size, size_t piece_size){
    size_t last_piece_size {calculateLastPieceSize(file_size, piece_size)};
    size_t block_size {calculateBlockSize(piece_size)};
    int begin_offset{};
    int piece_index{};
    while (begin_offset <= file_size){
      if(begin_offset + piece_size < file_size){
        m_all_pieces.emplace_back(piece_index, piece_size, block_size);
      }else{
        m_all_pieces.emplace_back(piece_index, last_piece_size, block_size);
      }
      begin_offset += piece_size;
      piece_index ++;
    }
  };
  void PieceManager::setPieceToDownload(){
    if(m_piece_is_set){
      return;
    }
    size_t rarest_piece_index{SIZE_MAX};
    size_t rarest_piece_owners{SIZE_MAX};
    for(size_t i{}; i< m_all_pieces.size(); i++){
      if(m_all_pieces[i].isCompleted()){
        continue;
      }
      if(m_connections[i].size() < rarest_piece_owners){
        rarest_piece_index = i;
        rarest_piece_owners = m_connections[i].size();
      }
    }
    //add error handling
    m_piece_to_download = rarest_piece_index;
    m_piece_is_set = true;
  };
  void PieceManager::startDownloadProcess(){
    if(m_piece_is_set || allPiecesAreCompleted()){
      return;
    }
    setPieceToDownload();
    bool all_are_choking{true};
    while(m_all_pieces[m_piece_to_download].hasBlockToDownload()){
      for(auto connection : m_connections[m_piece_to_download]){
        if(!connection.get().m_peer_choking){
          Block& block{m_all_pieces[m_piece_to_download].getBlockToDownload()};
          message::Action request{message::Params{message::length::REQUEST, message::ID::request},
                                  m_piece_to_download, block.getOffset(), block.getSize()};
          write::sendMessage(connection.get(), request);
          all_are_choking = false;
        }
      }
      if(all_are_choking) return; 
    }
    m_piece_is_set = false;
  }
};
