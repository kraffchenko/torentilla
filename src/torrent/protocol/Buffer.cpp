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
  void Buffer::fullMessageIsDelivered(bool full_message_is_delivered){
    m_full_message_is_delivered = full_message_is_delivered;
  };
  size_t Buffer::size(){
    return m_buffer.size();
  };
  size_t Buffer::capacity(){
    return m_buffer.capacity();
  };
  std::span<std::byte> Buffer::getRange(){
    return {m_buffer.data(), m_buffer.size()};
  };
  std::span<std::byte> Buffer::getRange(size_t start, size_t size){
    return {m_buffer.data() + start, size};
  };
  std::span<std::byte> Buffer::getAvailableRange(){
    if(m_full_message_is_delivered){
      return {m_buffer.data(), m_buffer.size()};
    }else{
      size_t filled_till {m_buffer.size()};
      m_buffer.resize(m_buffer.capacity());
      return {m_buffer.data() + (filled_till - 1), m_buffer.size()};
    }
  };
  std::byte& Buffer::operator[](size_t index){
    return m_buffer[index];
  }
}
