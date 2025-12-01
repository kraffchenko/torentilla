#ifndef TRACKERREQUEST_H
#define TRACKERREQUEST_H

#include <iostream>
#include <array>
#include <vector>

#include <curl/curl.h>
#include "torrent/Download.h"

namespace net{
  class Request{
  public:
    std::string url_encode(std::array<std::byte, 20> byte_array);
    std::vector<std::byte> trackerGetRequest(torrent::Download& download_structure, std::array<std::byte, 20> peer_id);
  private:
    static size_t callback(char* data, size_t size, size_t nmemb, void* clientp);
  };
}
#endif
