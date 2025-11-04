#include "Metainfo.h"

Metainfo::Metainfo(std::string_view announce, int64_t piece_length, std::string_view pieces, std::string_view name,
        int64_t length, std::optional<int64_t> creation_date=std::nullopt, std::optional<std::string_view> comment=std::nullopt, 
        std::optional<std::string_view> created_by=std::nullopt, std::optional<std::string_view> encoding=std::nullopt,
        std::optional<int> is_private=std::nullopt, std::optional<std::string_view> md5sum=std::nullopt)

        : m_announce{announce}, m_piece_length {piece_length}, m_pieces{pieces}, m_name{name}, m_length{length}, m_creation_date{creation_date},
          m_comment{comment}, m_created_by{created_by}, m_encoding{encoding}, m_is_private{is_private}, m_md5sum{md5sum}
          
        {
        };
