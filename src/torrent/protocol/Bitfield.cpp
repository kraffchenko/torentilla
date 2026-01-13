#include "torrent/protocol/Bitfield.h"

namespace torrent::protocol{
  bool Bitfield::hasPiece(int piece_index){
    auto block_and_bit {getBlockAndBitIndex(piece_index)};
    std::byte mask_byte{ std::byte{1} << static_cast<int>(block_and_bit.second)};
    return static_cast<bool>(m_bitfield.at(block_and_bit.first) & mask_byte);
  }
  std::pair<size_t, size_t> getBlockAndBitIndex(int piece_index){
    double piece_block_index{};
    double dec_part = std::modf(std::abs(piece_index / 8), &piece_block_index);
    int bit_index {static_cast<int>(dec_part * 10)};
    std::pair<size_t, size_t> block_and_bit { static_cast<size_t>(piece_block_index),
                                              static_cast<size_t>(bit_index)};
    return block_and_bit;
  }
  std::string Bitfield::toString(){
    return{std::string{reinterpret_cast<char*>(m_bitfield.data())}};
  }
  Bitfield Bitfield::fromString(std::string_view bitfield_string, const size_t piece_amnt){
    std::vector<std::byte> bitfield(bitfield_string.size());
    std::transform(bitfield_string.begin(), bitfield_string.end(), bitfield.begin(),
                   [](char c) { return std::byte(c);});
    return Bitfield{bitfield, piece_amnt};
  }
}
