#include "torrent/protocol/Bitfield.h"

namespace torrent::protocol{
  bool Bitfield::hasPiece(int piece_index){
    auto block_and_bit {getBlockAndBitIndex(piece_index)};
    std::byte mask_byte{ std::byte{1} << static_cast<int>(block_and_bit.second)};
    return static_cast<bool>(m_bitfield.at(block_and_bit.first) & mask_byte);
  }
  std::pair<size_t, size_t> Bitfield::getBlockAndBitIndex(int piece_index){
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
  std::vector<size_t> getMissingPieces(std::vector<std::byte>& local_bitfield_array, 
                                       std::vector<std::byte>& remote_bitfield_array){
    std::vector<size_t> missing_pieces{};
    for(size_t i{}; i < local_bitfield_array.size(); i++){
      if(local_bitfield_array[i] != remote_bitfield_array[i]){
        std::byte missing_pieces_byte{~local_bitfield_array[i] & remote_bitfield_array[i]}; 
        std::bitset<8> bit{static_cast<unsigned char>(missing_pieces_byte)};
        size_t starting_piece_index {i*8}; 
        for(size_t bi{}; bi < 8; bi++){
          if(bit.test(bi)){
            missing_pieces.push_back(starting_piece_index + bi);
          }
        }
      }
    }
    return missing_pieces;
  }
}
