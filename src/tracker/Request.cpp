#include "tracker/Request.h"
namespace {
  size_t callback(char* data, size_t size, size_t nmemb, void* clientp){
    std::vector<std::byte>* byte_array{static_cast<std::vector<std::byte>*>(clientp)};
    const std::byte* casted_data_pointer {reinterpret_cast<const std::byte*>(data)};
    byte_array -> insert(byte_array->end(), casted_data_pointer, casted_data_pointer + size*nmemb);
    return size * nmemb;
  }
}
namespace tracker{
  Request::Request(const torrent::dottorrent::Metadata& data, int64_t left)
  : m_requested_torrent{data}, m_left{left}
  {

  }
  Request::Request(const torrent::dottorrent::Metadata& requested_torrent, int port, int64_t uploaded, 
                   int64_t downloaded, int64_t left, bool compact, 
                   bool no_peer_id, std::string& event)
  : m_requested_torrent{requested_torrent}, m_port{port},
    m_uploaded{uploaded}, m_downloaded{downloaded},
    m_left{left}, m_compact{compact}, m_no_peer_id{no_peer_id},
    m_event{event}
  {

  }
  std::vector<std::byte> Request::sendGetRequest(std::array<std::byte, 20>& peer_id){
    std::vector<std::byte> response{};
    std::string request_url{
      std::string{m_requested_torrent.getAnnounce()}
      + "?info_hash=" + net::utils::url_encode(m_requested_torrent.getInfoHash())
      + "&peer_id=" + net::utils::url_encode(peer_id)
      + "&port=" + std::to_string(m_port) 
      + "&uploaded=" + std::to_string(m_uploaded)
      + "&downloaded=" + std::to_string(m_downloaded)
      + "&left=" + std::to_string(m_left)
      + "&compact=" + std::to_string(m_compact)
      + "&no_peer_id=" + std::to_string(m_no_peer_id)
      + "&event=" + m_event
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
