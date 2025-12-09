#ifndef REQUEST_H
#define REQUEST_H
#include <iostream>
#include <array>
#include <vector>
#include "torrent/dottorrent/Metadata.h"
#include "net/utils/utils.h"
namespace tracker {
  class Request{
    private:
      const torrent::dottorrent::Metadata& m_requested_torrent;
      int m_port{6881};
      int64_t m_uploaded{0};
      int64_t m_downloaded{0};
      int64_t m_left;
      bool m_compact{true};
      bool m_no_peer_id{true};
      std::string m_event{"started"};
    public:
      Request(const torrent::dottorrent::Metadata& requested_torrent, int64_t left);
      Request(const torrent::dottorrent::Metadata& requested_torrent, int port, int64_t uploaded, 
              int64_t donwloaded, int64_t left, bool compact, bool no_peer_id, std::string& event);
      std::vector<std::byte> sendGetRequest(std::array<std::byte, 20>& peer_id);
    };
}
#endif
