#include "TrackerRequest.h"

std::string TrackerRequest::url_encode(std::array<std::byte, 20> byte_array){
  CURL* curl{curl_easy_init()};
  std::string encoded_string{};
  if(curl){
    char* encoded_byte_array{curl_easy_escape(curl, reinterpret_cast<char*>(byte_array.data()), 20)};
    if(encoded_byte_array){
      encoded_string = encoded_byte_array;
      curl_free(encoded_byte_array);
    }
    curl_easy_cleanup(curl);
  }
  return encoded_string;
}
 size_t TrackerRequest::callback(char* data, size_t size, size_t nmemb, void* clientp){
  std::vector<std::byte>* byte_array{static_cast<std::vector<std::byte>*>(clientp)};
  const std::byte* casted_data_pointer {reinterpret_cast<const std::byte*>(data)};
  byte_array -> insert(byte_array->end(), casted_data_pointer, casted_data_pointer + size*nmemb);
  return size * nmemb;
}
std::vector<std::byte> TrackerRequest::trackerGetRequest(TorrentDownload& download_structure, std::array<std::byte, 20> peer_id){
  std::vector<std::byte> response{};
  std::string request_url{
    std::string{download_structure.getAnnounce()}
    + "?info_hash=" + url_encode(download_structure.getInfoHash())
    + "&peer_id=" + url_encode(peer_id)
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

