#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <iostream>
#include <array>
#include <vector>
#include <bitset>
namespace torrent{
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
  class Communication{
  private:
    bool am_choking{true};
    bool am_interested{false};
    bool peer_choking{true};
    bool peer_interested{false};
  public:
    void sendHandshake(std::array<std::byte, 20>& info_hash, std::array<std::byte, 20>& peer_id);
  };
  template<MessageID id> 
  struct Message{
    Message() = delete;
  };
  template<>
  struct Message<MessageID::choke>{
    static void sendMessage(){
    };
  };
  template<>
  struct Message<MessageID::unchoke>{
    static void sendMessage(){
    };
  };  
  template<>
  struct Message<MessageID::interested>{
    static void sendMessage(){
    };
  };
  template<>
  struct Message<MessageID::not_interested>{
    static void sendMessage(){
    };
  };
  template<>
  struct Message<MessageID::have>{
    static void sendMessage(int piece_index){
    };
  };
  template<>
  struct Message<MessageID::bitfield>{
    void sendMessage(int bitfield_length){
    };
  };
  template<>
  struct Message<MessageID::request>{
    static void sendMessage(int index, int begin, int length){
    };
  };
  template<>
  struct Message<MessageID::piece>{
    static void sendMessage(int index, int begin, std::vector<std::byte> block){
    };
  };
  template<>
  struct Message<MessageID::cancel>{
    static void sendMessage(int index, int begin, int length){
    };
  };
}
#endif
