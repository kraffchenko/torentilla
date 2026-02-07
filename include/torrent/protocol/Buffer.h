#ifndef BUFFER_H
#define BUFFER_H
#include <iostream>
#include <vector>
#include <span>
namespace torrent::protocol{ 
  class Buffer{
  public:
    std::span<std::byte> getAvailableRange();
    std::span<std::byte> getRange();
    std::span<std::byte> getRange(size_t start, size_t end);
    bool fullMessageIsDelivered();
    bool isProcessingMessage();
    void processing(bool is_processing_a_message);
    void delivering(bool contains_full_message);
    void reserve(size_t capacity);
    void resize(size_t size);
    void reset(size_t processed_bytes);
    void setFilled(size_t filled);
    void setLength(size_t length);
    size_t size();
    size_t filled();
    size_t length();
    size_t capacity();
    Buffer(size_t size);
    Buffer();
    std::byte& operator[](size_t index);
  private:
    static constexpr size_t m_default_buffer_size{1024};
    std::vector<std::byte> m_buffer{};
    size_t m_filled{};
    size_t m_length{};
    bool m_processing_a_message{false};
    bool m_full_message_is_delivered{false};
  };
}
#endif
