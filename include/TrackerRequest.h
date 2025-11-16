#ifndef TRACKERREQUEST_H
#define TRACKERREQUEST_H

#include <curl/curl.h>
#include "Metainfo.h"

class TrackerRequest{
public:
  std::string url_encode(std::array<std::byte, 20> byte_array);
private:
};

#endif
