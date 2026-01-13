#include "torrent/protocol/Piece.h"
namespace torrent::protocol{
  Piece::Piece(int32_t index)
  : m_index{index}
  {
  }
  void Piece::setOffset(int32_t offset){
    m_current_offset = offset;
  }
  int32_t Piece::getIndex(){
    return m_index;
  };
  int32_t Piece::getCurrentOffset(){
    return m_current_offset;
  };
}
