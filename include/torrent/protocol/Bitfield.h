#ifndef BITFIELD_H
#define BITFIELD_H
#include <iostream>
#include <cmath>
#include <vector>

namespace torrent::protocol{
  class Bitfield{
  public:
    std::vector<std::byte>& getBitfield(){ return m_bitfield; };
    size_t getPiecesAmount() { return m_pieces_amnt; }
    bool hasPiece(int piece_index);
    std::string toString();
    static Bitfield fromString(std::string_view bitfield_string, const size_t piece_amnt);
    std::pair<size_t, size_t> getBlockAndBitIndex(int piece_index);
    Bitfield(size_t pieces_amnt) : m_pieces_amnt{pieces_amnt}, m_bitfield((pieces_amnt + 8 - 1) / 8){};
  private:
    Bitfield(std::vector<std::byte>& bitfield, const size_t piece_amnt);
    size_t m_pieces_amnt{};
    std::vector<std::byte> m_bitfield{};
  };
}
#endif
