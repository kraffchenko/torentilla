#include "torrent/protocol/PieceManager.h"
namespace torrent::protocol{
  PieceManager::PieceManager(size_t file_size, size_t piece_size){
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
  std::vector<std::reference_wrapper<net::Connection>>& PieceManager::getConnectionsByIndex(int index){
   return m_connections.at(index); 
  };
  void PieceManager::addConnection(size_t index, net::Connection& connection){
    m_connections.at(index).push_back(connection);
  };
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

};
