#ifndef PIECE_H
#define PIECE_H
#include <cstdint>
#include <vector>
#include <algorithm>
namespace torrent::protocol{
  static constexpr int32_t DEFAULT_PIECE_LENGTH_B{524288};
  static constexpr int32_t DEFAULT_BLOCK_LENGTH_B{16384};
  inline size_t calculateLastPieceSize(size_t file_size, size_t piece_size){
    return file_size % piece_size;
  };
  inline size_t calculateBlockSize(size_t piece_size){
    if(piece_size >= DEFAULT_BLOCK_LENGTH_B){
      return DEFAULT_BLOCK_LENGTH_B;  
    }else{
      return piece_size;
    }
  };
  inline size_t calculateLastBlockSize(size_t block_size, size_t piece_size){
    return piece_size % block_size;
  };
  inline std::pair<size_t, size_t> getPieceBlockSizes(size_t file_size, size_t piece_size){
    size_t l_piece_size{calculateLastPieceSize(file_size, piece_size)};
    if(l_piece_size > 0){
      if(l_piece_size <= DEFAULT_BLOCK_LENGTH_B){
        return {l_piece_size, l_piece_size};
      }else{
        return {l_piece_size, DEFAULT_BLOCK_LENGTH_B};
      }
    }else{
      return {DEFAULT_PIECE_LENGTH_B, DEFAULT_BLOCK_LENGTH_B};
    }
  };
  class Block{
  public:
    bool m_being_downloaded{false};
    bool m_was_downloaded{false};
    size_t getSize() { return m_size; };
    int64_t getOffset() { return m_begin_offset; };
    Block(int32_t begin_offset, size_t size) : m_size{size}, m_begin_offset{begin_offset}{};
    bool operator == (Block& block) const{
      return m_begin_offset == block.getOffset();
    }
  private:
    size_t m_size{};
    int32_t m_begin_offset{};
  };
  class Piece{
  public:
    size_t getIndex();
    size_t getPieceSize();
    bool isCompleted();
    bool hasBlockToDownload();
    Block& getBlockToDownload();
    bool markBlockAsDone(Block& block);
    bool markBlockAsDone(Block&& block);
    Piece(int32_t index, size_t piece_size, size_t block_size);
  private:
    std::vector<Block> m_blocks{};
    int m_blocks_remained{};
    int m_pending_blocks{};
    int32_t m_index{};
    size_t m_piece_size{};
    void initializeBlocks(size_t block_size);
  };
};
#endif
