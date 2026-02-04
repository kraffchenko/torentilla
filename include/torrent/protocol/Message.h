#ifndef MESSAGE_H
#define MESSAGE_H
#include <cstdint>
#include <vector>
#include <boost/asio.hpp>
namespace torrent::protocol::message{

  enum class ID{
    choke,
    unchoke,
    interested,
    not_interested,
    have,
    bitfield,
    request,
    piece,
    cancel, 
  };
  namespace BytePos{
    static constexpr size_t PSTRLEN{0};
    static constexpr size_t PSTR{1};
    static constexpr size_t RESERVED{20};
    static constexpr size_t INFO_HASH{28};
    static constexpr size_t PEER_ID{48};
    static constexpr size_t LEN{0};
    static constexpr size_t ID{4};
    static constexpr size_t INDEX{5};
    static constexpr size_t BITFIELD{5};
    static constexpr size_t BEGIN{9};
    static constexpr size_t REQUESTED_LENGTH{13};
    static constexpr size_t BLOCK{13};
  };
  namespace length{
    static constexpr int32_t STATE{1};
    static constexpr int32_t HAVE{5};
    static constexpr int32_t REQUEST{13};
    static constexpr int32_t PIECE{9};
    static constexpr int32_t CANCEL{13};
  };
  inline ID getMessageID(std::vector<std::byte>& buffer){
    return static_cast<ID>(buffer.at(BytePos::ID));
  };
  template<typename T>
  inline int getIntFromBytes(T iterator){
    uint32_t num{};
    std::memcpy(&num, iterator, 4);
    num = ntohl(num);
    return static_cast<int>(num);
  };
  template<typename T>
  inline std::vector<std::byte> getBytesInRange(T iterator, size_t message_length){
    std::vector<std::byte> bytes_array{};
    bytes_array.reserve(message_length);
    std::copy(iterator, iterator+message_length, std::back_inserter(bytes_array));  
    return bytes_array;
  };
  inline std::array<std::byte, 4> getByteArrayFromInt(uint32_t num){
    std::array<std::byte, 4> byte_array{};
    uint32_t number{htonl(num)};
    std::memcpy(byte_array.data(), &number, 4);
    return byte_array;
  };
  struct Params{
    int64_t message_len{};
    ID id{};
  };
  struct Handshake{
    std::byte pstrlen{static_cast<std::byte>(19)};
    std::string pstr{"BitTorrent protocol"};
    std::array<std::byte, 8> reserved{};
    std::array<std::byte, 20> info_hash{};
    std::array<std::byte, 20> peer_id{};
    
    bool operator == (const Handshake& handshake) const{
       return pstrlen == handshake.pstrlen &&
              pstr == handshake.pstr && 
              info_hash == handshake.info_hash; 
    };
    std::vector<std::byte> inByteArray(){
      std::vector<std::byte> byte_array{};
      byte_array.push_back(pstrlen);
      std::transform(pstr.begin(), pstr.end(), std::back_inserter(byte_array), [](char ch){ return static_cast<std::byte>(ch); });
      std::copy(reserved.begin(), reserved.end(), std::back_inserter(byte_array));
      std::copy(info_hash.begin(), info_hash.end(), std::back_inserter(byte_array));
      std::copy(peer_id.begin(), peer_id.end(), std::back_inserter(byte_array)); 
      return byte_array;
    };
  };
  struct KeepAlive{
    int32_t len{};
  };
  struct State{
    Params msg_params{};
    std::vector<std::byte> inByteArray(){
      std::vector<std::byte> byte_array{};
      std::array<std::byte, 4> message_len{getByteArrayFromInt(msg_params.message_len)};
      byte_array.insert(byte_array.end(), message_len.begin(), message_len.end());
      byte_array.push_back(static_cast<std::byte>(msg_params.id));
      return byte_array;
    };
  };
  struct Have{
    Params msg_params{};
    int piece_index{};
    std::vector<std::byte> inByteArray(){
      std::vector<std::byte> byte_array{};
      std::array<std::byte, 4> message_len{getByteArrayFromInt(msg_params.message_len)};
      byte_array.insert(byte_array.end(), message_len.begin(), message_len.end());
      byte_array.push_back(static_cast<std::byte>(msg_params.id));
      std::array<std::byte, 4> index{getByteArrayFromInt(piece_index)};
      byte_array.insert(byte_array.end(),index.begin(), index.end());
      return byte_array;
    };
  };
  struct Bitfield{
    Params msg_params{};
    std::vector<std::byte>bitfield_as_byte_array{};
    std::vector<std::byte> inByteArray(){
      std::vector<std::byte> byte_array{};
      std::array<std::byte, 4> message_len{getByteArrayFromInt(msg_params.message_len)};
      byte_array.insert(byte_array.end(), message_len.begin(), message_len.end());
      byte_array.push_back(static_cast<std::byte>(msg_params.id));
      byte_array.insert(byte_array.end(), bitfield_as_byte_array.begin(), bitfield_as_byte_array.end());
      return byte_array;
    };
  };
  struct Action{
    Params msg_params{};
    int index{};
    int begin{};
    int requested_length{};
    std::vector<std::byte> inByteArray(){
      std::vector<std::byte> byte_array{};
      std::array<std::byte, 4> message_len{getByteArrayFromInt(msg_params.message_len)};
      byte_array.insert(byte_array.end(), message_len.begin(), message_len.end());
      byte_array.push_back(static_cast<std::byte>(msg_params.id));
      std::array<std::byte, 4> l_index{getByteArrayFromInt(index)};
      byte_array.insert(byte_array.end(), l_index.begin(), l_index.end());
      std::array<std::byte, 4> l_begin{getByteArrayFromInt(begin)};
      byte_array.insert(byte_array.end(), l_begin.begin(), l_begin.end());
      std::array<std::byte, 4> l_requested_length{getByteArrayFromInt(requested_length)};
      byte_array.insert(byte_array.end(), l_requested_length.begin(), l_requested_length.end());
      return byte_array;
    };
  };
  struct Piece{
    Params msg_params{};
    int index{};
    int begin{};
    std::vector<std::byte> block{};
    std::vector<std::byte> inByteArray(){
      std::vector<std::byte> byte_array{};
      std::array<std::byte, 4> message_len{getByteArrayFromInt(msg_params.message_len)};
      byte_array.insert(byte_array.end(), message_len.begin(), message_len.end());
      byte_array.push_back(static_cast<std::byte>(msg_params.id));
      std::array<std::byte, 4> l_index{getByteArrayFromInt(index)};
      byte_array.insert(byte_array.end(), l_index.begin(), l_index.end());
      std::array<std::byte, 4> l_begin{getByteArrayFromInt(begin)};
      byte_array.insert(byte_array.end(), l_begin.begin(), l_begin.end());
      byte_array.insert(byte_array.end(), block.begin(), block.end());
      return byte_array;
    };
  };
  inline Handshake createHandshakeFromBuffer(std::vector<std::byte>& buffer){
      std::byte pstrlen {buffer[BytePos::PSTRLEN]};
      std::string pstr {};
      pstr.resize(19);
      std::transform(&buffer[BytePos::PSTR], &buffer[BytePos::PSTR] + 19, 
                        pstr.begin(), [](std::byte bt) {return static_cast<char>(bt); });
      std::array<std::byte, 8> reserved{};
      std::copy(&buffer[BytePos::RESERVED], &buffer[BytePos::RESERVED] + 8, reserved.begin());
      std::array<std::byte, 20> info_hash{};
      std::copy(&buffer[BytePos::INFO_HASH], &buffer[BytePos::INFO_HASH] + 20 , info_hash.begin());
      std::array<std::byte, 20> peer_id {};
      std::copy(&buffer[BytePos::PEER_ID], &buffer[BytePos::PEER_ID] + 20, peer_id.begin());
      return Handshake{pstrlen, pstr, reserved, info_hash, peer_id};
  };
  using Message = std::variant<State,
                              Have,
                              Bitfield,
                              Action,
                              Piece>;
  inline Message createMessageFromBuffer(ID message_id, int message_length, std::byte* buffer){
    switch(message_id){
      case ID::choke:
      case ID::unchoke:
      case ID::interested:
      case ID::not_interested:
        return State{Params{message_length, message_id}};
      case ID::have:
        return Have{Params{message_length, message_id}, 
                          getIntFromBytes(&buffer[BytePos::INDEX])};
      case ID::bitfield:
        return Bitfield{Params{message_length, message_id}, 
                               getBytesInRange(&buffer[BytePos::BITFIELD], message_length)};
      case ID::request:
      case ID::cancel:
        return Action{Params{message_length, message_id}, 
                            getIntFromBytes(&buffer[BytePos::INDEX]),
                            getIntFromBytes(&buffer[BytePos::BEGIN]),
                            getIntFromBytes(&buffer[BytePos::REQUESTED_LENGTH])};
      case ID::piece:
        return Piece{Params{message_length, message_id},
                            getIntFromBytes(&buffer[BytePos::INDEX]),
                            getIntFromBytes(&buffer[BytePos::BEGIN]),
                            getBytesInRange(&buffer[BytePos::BLOCK], message_length)};
      default:
        throw std::runtime_error("Unknown message ID in createMessageFromBuffer");
    };
  };
};

#endif

