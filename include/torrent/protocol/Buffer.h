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
    void fullMessageIsDelivered(bool contains_full_message);
    void reserve(size_t capacity);
    size_t size();
    size_t capacity();
    Buffer(size_t size);
    Buffer();
    std::byte& operator[](size_t index);
  private:
    static constexpr size_t m_default_buffer_size{1024};
    std::vector<std::byte> m_buffer{};
    bool m_full_message_is_delivered{false};
  };
}
#endif
