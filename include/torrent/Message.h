#ifndef MESSAGE_H
#define MESSAGE_H
#include <cstdint>
#include <vector>
#include <boost/asio.hpp>
namespace torrent::message{
  enum class MessageID{
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
  struct MessageParams{
    int32_t message_len{};
    MessageID id{};
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
              reserved == handshake.reserved &&
              info_hash == handshake.info_hash;
    }
  };
  struct KeepAliveMessage{
    int32_t len{};
  };
  struct StateMessage{
    MessageParams msg_params{};
  };
  struct HaveMessage{
    MessageParams msg_params{};
    int piece_index{};
  };
  struct BitfieldMessage{
    MessageParams msg_params{};
    std::vector<std::byte> bitfield{};
  };
  struct ActionMessage{
    MessageParams msg_params{};
    int index{};
    int begin{};
    int requested_length{};
  };
  struct PieceMessage{
    MessageParams msg_params{};
    int index{};
    int begin{};
    std::vector<std::byte> block{};
  };
  
  inline Handshake createHandshakeFromBuffer(std::vector<std::byte>& buffer){
      std::byte pstrlen {buffer[BytePos::PSTRLEN]};
      std::string pstr {};
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

  using Message = std::variant<torrent::message::StateMessage,
                              torrent::message::HaveMessage,
                              torrent::message::BitfieldMessage,
                              torrent::message::ActionMessage,
                              torrent::message::PieceMessage>;

  inline MessageID getMessageID(std::vector<std::byte>& buffer){
    return static_cast<MessageID>(buffer.at(BytePos::ID));
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
    std::copy(iterator, iterator+message_length, bytes_array.data());  
    return bytes_array;
  };
  inline Message createMessageFromBuffer(MessageID message_id, int message_length, std::vector<std::byte>& buffer){
    switch(message_id){
      case MessageID::choke:
      case MessageID::unchoke:
      case MessageID::interested:
      case MessageID::not_interested:
        return StateMessage{MessageParams{message_length, message_id}};
      case MessageID::have:
        return HaveMessage{MessageParams{message_length, message_id}, 
                          getIntFromBytes(&buffer[BytePos::INDEX])};
      case MessageID::bitfield:
        return BitfieldMessage{MessageParams{message_length, message_id}, 
                              getBytesInRange(&buffer[BytePos::BITFIELD], message_length)};
      case MessageID::request:
      case MessageID::cancel:
        return ActionMessage{MessageParams{message_length, message_id}, 
                            getIntFromBytes(&buffer[BytePos::INDEX]),
                            getIntFromBytes(&buffer[BytePos::BEGIN]),
                            getIntFromBytes(&buffer[BytePos::REQUESTED_LENGTH])};
      case MessageID::piece:
        return PieceMessage{MessageParams{message_length, message_id},
                            getIntFromBytes(&buffer[BytePos::INDEX]),
                            getIntFromBytes(&buffer[BytePos::BEGIN]),
                            getBytesInRange(&buffer[BytePos::BLOCK], message_length)};
    };
  };
}

#endif

