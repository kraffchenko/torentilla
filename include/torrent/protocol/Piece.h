#ifndef PIECE_H
#define PIECE_H
#include <cstdint>
#include <vector>
#include <array>
namespace torrent::protocol{
  class Piece{
  public:
    static constexpr int32_t PIECE_LENGTH_B{524288};
    static constexpr int32_t BLOCK_LENGTH_B{16384};
    int32_t getIndex();
    int32_t getCurrentOffset();
    void setOffset(int32_t offset);
    void moveToByteArray(std::vector<std::byte>::const_iterator start,
                         std::vector<std::byte>::const_iterator end);
    std::pair<std::array<std::byte, PIECE_LENGTH_B>::iterator,
              std::array<std::byte, PIECE_LENGTH_B>::iterator> getBlock(); 
    Piece(int32_t index);
  private:
    int32_t m_index{};
    int32_t m_current_offset{};
    std::array<std::byte, PIECE_LENGTH_B> m_piece{};  
    std::array<std::byte, PIECE_LENGTH_B>::iterator m_begin {m_piece.begin()};
  };
};
#endif
