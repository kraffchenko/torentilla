#include <iostream>
#include <vector>
#include "net/utils/utils.h"
namespace net{  
  inline size_t callback(char* data, size_t size, size_t nmemb, void* clientp){
    std::vector<std::byte>* byte_array{static_cast<std::vector<std::byte>*>(clientp)};
    const std::byte* casted_data_pointer {reinterpret_cast<const std::byte*>(data)};
    byte_array -> insert(byte_array->end(), casted_data_pointer, casted_data_pointer + size*nmemb);
    return size * nmemb;
  }
  inline std::vector<std::byte> trackerGetRequest(torrent::Download& download_structure, std::array<std::byte, 20> peer_id){
    std::vector<std::byte> response{};
    std::string request_url{
      std::string{download_structure.getAnnounce()}
      + "?info_hash=" + net::utils::url_encode(download_structure.getInfoHash())
      + "&peer_id=" + net::utils::url_encode(peer_id)
      + "&port=" + download_structure.getPort()
      + "&uploaded=" + std::to_string(download_structure.getUploaded())
      + "&downloaded=" + std::to_string(download_structure.getDownloaded())
      + "&left=" + std::to_string(download_structure.getLeft())
      + "&compact=" + std::to_string(download_structure.getCompact())
      + "&no_peer_id=" + std::to_string(download_structure.getNoPeerId())
      + "&event=" + download_structure.getEvent()
    };
    CURL *curl{curl_easy_init()};
    if(curl){
      curl_easy_setopt(curl, CURLOPT_URL, request_url.c_str());
      curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
      CURLcode result {curl_easy_perform(curl)};
      std::cout << result << '\n';
      curl_easy_cleanup(curl);    
    }
    for (std::byte byte : response){
      std::cout << static_cast<char>(byte) << '\n';
    }
    return response;
  }
}
