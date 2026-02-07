#include "torrent/protocol/Buffer.h"

namespace torrent::protocol{
  Buffer::Buffer(size_t size)
  : m_buffer(size)
  { 
    m_buffer.resize(size);
  };
  Buffer::Buffer()
  : m_buffer(m_default_buffer_size)
  {
    m_buffer.resize(m_default_buffer_size);
  };
  void Buffer::reserve(size_t capacity){
    m_buffer.reserve(capacity);
  };
  void Buffer::resize(size_t size){
    m_buffer.resize(size);
  };
  void Buffer::reset(size_t processed_bytes){
    m_buffer.resize(m_default_buffer_size);
    m_length = 0;
    m_filled -= processed_bytes;
  };
  void Buffer::delivering(bool full_message_is_delivered){
    m_full_message_is_delivered = full_message_is_delivered;
  };
  void Buffer::processing(bool is_processing_message){
    m_processing_a_message = is_processing_message;
  } 
  bool Buffer::fullMessageIsDelivered(){
    return m_full_message_is_delivered;
  };
  bool Buffer::isProcessingMessage(){
    return m_processing_a_message;
  } 
  size_t Buffer::size(){
    return m_buffer.size();
  };
  size_t Buffer::capacity(){
    return m_buffer.capacity();
  };
  size_t Buffer::filled(){
    return m_filled;
  };
  void Buffer::setFilled(size_t filled){
    m_filled += filled;
  };
  size_t Buffer::length(){
    return m_length;
  }
  void Buffer::setLength(size_t length){
    m_length = length;
  };
  std::span<std::byte> Buffer::getRange(){
    return {m_buffer.data(), m_buffer.size()};
  };
  std::span<std::byte> Buffer::getRange(size_t start, size_t size){
    return {m_buffer.data() + start, size};
  };
  std::span<std::byte> Buffer::getAvailableRange(){
    if(length() > 0){
      return {m_buffer.data() + filled(), length()};
    }else{
      return {m_buffer.data(), m_buffer.size()};
    }
  };
  std::byte& Buffer::operator[](size_t index){
    return m_buffer[index];
  }
}
