#include "torrent/protocol/Piece.h"
namespace torrent::protocol{
  Piece::Piece(int32_t index, size_t piece_size, size_t block_size)
  : m_index{index}, m_piece_size{piece_size}  
  {
    initializeBlocks(block_size);
    m_blocks.shrink_to_fit();
    m_blocks_remained = m_blocks.size();
  }
  int32_t Piece::getIndex(){
    return m_index;
  };
  bool Piece::isCompleted(){
    if(m_blocks_remained != 0){
      return false;
    }else{
      return true;
    }
  };
  size_t Piece::getPieceSize(){
    return m_piece_size; 
  };
  void Piece::initializeBlocks(size_t block_size){
    size_t last_block_size {calculateLastBlockSize(block_size, m_piece_size)};
    int begin_offset{};
    while(begin_offset <= m_piece_size){
      if(begin_offset + block_size < m_piece_size){
        m_blocks.emplace_back(begin_offset, block_size);
      }else{
        m_blocks.emplace_back(begin_offset, last_block_size);
      }
      begin_offset+=block_size;
    }
  }
  bool Piece::hasBlockToDownload(){
   if(m_pending_blocks != 0){
      return true;
    }else{
      return false;
    }
  }
  Block& Piece::getBlockToDownload(){
    for(Block& block : m_blocks){
      if(!block.m_being_downloaded){
        block.m_being_downloaded = true;
        m_pending_blocks += 1;
        return block;
      }
    }
  }
  void Piece::markBlockAsDone(Block& block){
    auto it{std::find(m_blocks.begin(),
                      m_blocks.end(),
                      block)};
    block.m_being_downloaded = false;
    block.m_was_downloaded = true;
    m_blocks_remained -= 1;
  }

}
