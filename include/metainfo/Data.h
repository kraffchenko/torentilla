#ifndef METAINFO_H
#define METAINFO_H

#include <iostream>
#include <vector>
#include <optional>
#include <random>
#include <array>

namespace metainfo{
class Data {
  public:
    Data(std::string_view announce, int64_t piece_length, std::string_view pieces, std::string_view name,
             int64_t length, std::array<std::byte, 20> info_hash, std::optional<std::vector<std::vector<std::string>>> announce_list, std::optional<int64_t> creation_date, std::optional<std::string_view> comment, std::optional<std::string_view> created_by, std::optional<std::string_view> encoding,
             std::optional<int> is_private, std::optional<std::string_view> md5sum);
    Data() = delete;

    const std::string_view getAnnounce() const { return m_announce; };
    const int64_t getPieceLength() const { return m_piece_length; };
    const std::string_view getPieces() const { return m_pieces; };
    const std::string_view getName() const { return m_name; };
    const int64_t getLength() const { return m_length; };
    std::array<std::byte, 20> getInfoHash() const { return m_info_hash; }; 

    const std::optional<std::string_view> getComment() const { return m_comment; };
    const std::optional<std::string_view> getEncoding() const { return m_encoding; };
    const std::optional<std::string_view> getCreatedBy() const { return m_created_by; };
    const std::optional<std::vector<std::vector<std::string>>> getAnnounceList() const { return m_announce_list;};
    const std::optional<int> getPrivateStatus() const { return m_is_private; };
    const std::optional<std::string_view> getMd5sum() const { return m_md5sum; };
    const std::optional<int64_t> getCreationDate() { return m_creation_date; }


  private:
    const std::string m_announce{};  
    const std::string m_name{};
    const int64_t m_piece_length{};
    const std::string m_pieces{};
    const int64_t m_length{};
    std::array<std::byte, 20> m_info_hash{};

    const std::optional<std::vector<std::vector<std::string>>> m_announce_list{};
    const std::optional<int64_t> m_creation_date{};
    const std::optional<std::string> m_comment{};
    const std::optional<std::string> m_created_by{};
    const std::optional<std::string> m_encoding{};
    const std::optional<int> m_is_private{};
    const std::optional<std::string> m_md5sum{};
  };
}
#endif
