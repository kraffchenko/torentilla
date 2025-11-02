#ifndef METAINFO_H
#define METAINFO_H

#include <iostream>
#include <vector>
#include <optional>

class Metainfo {
public:
  Metainfo(std::string_view announce, int64_t piece_length, std::vector<std::byte>& pieces, std::string_view name,
           int64_t length, std::optional<int64_t> creation_date, std::optional<std::string_view> comment, std::optional<std::string_view> created_by, std::optional<std::string_view> encoding,
           std::optional<int> is_private, std::optional<std::reference_wrapper<std::vector<std::byte>>> md5sum);
  Metainfo() = delete;

  std::string_view getAnnounce() const { return m_announce; };
  int64_t getPieceLength() const { return m_piece_length; };
  const std::vector<std::byte>& getPieces() const { return m_pieces; };
  std::string_view getName() const { return m_name; };
  int64_t getLength() const { return m_length; };

  std::optional<std::string_view> getComment() const { return m_comment; };
  std::optional<std::string_view> getEncoding() const { return m_encoding; };
  std::optional<std::string_view> getCreatedBy() const { return m_created_by; };
  std::optional<int> getPrivateStatus() const { return m_is_private; };
  const std::optional<std::vector<std::byte>> getMd5sum() const { return m_md5sum; };
  std::optional<int64_t> getCreationDate() { return m_creation_date; }

private:
  const std::string m_announce{};  
  const std::string m_name{};
  const int64_t m_piece_length{};
  const std::vector<std::byte> m_pieces{};
  const int64_t m_length{};
  
  const std::optional<int64_t> m_creation_date{};
  const std::optional<std::string> m_comment{};
  const std::optional<std::string> m_created_by{};
  const std::optional<std::string> m_encoding{};
  const std::optional<int> m_is_private{};
  const std::optional<std::vector<std::byte>> m_md5sum{};
};

#endif
