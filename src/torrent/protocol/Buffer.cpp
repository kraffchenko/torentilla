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
  void Buffer::move(std::vector<std::byte>&& payload){
    setLength(payload.size());
    std::cout << "Buffer::move: Buffer length set to " << length() << " bytes."<< '\n';
    setFilled(payload.size());
    std::cout << "Buffer::move: Buffer is filled up to " << filled() << " bytes." << '\n'; 
    m_buffer = std::move(payload);
    std::cout << "Buffer::move: payload was moved into the buffer." << '\n';
  };
  void Buffer::insert(std::vector<std::byte>&& payload){
    if(size() < length() + payload.size()){
      std::cout << "Buffer::append: Resizing buffer to match the new length..." << '\n';
      m_buffer.resize(length() + payload.size());
      std::cout << "Buffer::append: Buffer resized to " << size() << " bytes." << '\n'; 
    }
    setLength(length() + payload.size());
    std::cout << "Buffer::insert: Buffer length set to " << length() << " bytes."<< '\n';
    m_buffer.insert(m_buffer.begin() + filled(), 
                    std::make_move_iterator(payload.begin()), std::make_move_iterator(payload.end()));
    setFilled(filled() + payload.size());
    std::cout << "Buffer::insert: Buffer is filled up to " << filled() << " bytes." << '\n';
  };
  void Buffer::reserve(size_t capacity){
    m_buffer.reserve(capacity);
  };
  void Buffer::resize(size_t size){
    m_buffer.resize(size);
  };
  void Buffer::rotate(size_t amount){
    std::rotate(m_buffer.begin(),m_buffer.begin() + amount, m_buffer.end());
  };
  void Buffer::reset(size_t processed_bytes){
    if(m_buffer.size() > m_default_buffer_size){
      m_buffer.resize(m_default_buffer_size);
      std::cout << "Buffer::reset: Resizing buffer back to the default size..." << '\n';
    }
    processing(false);
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
