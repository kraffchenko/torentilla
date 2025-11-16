#include "TrackerRequest.h"

std::string TrackerRequest::url_encode(std::array<std::byte, 20> byte_array){
  CURL* curl{curl_easy_init()};
  std::string encoded_string{};
  if(curl){

    char* encoded_byte_array{curl_easy_escape(curl, reinterpret_cast<char*>(byte_array.data()), 20)};
    if(encoded_byte_array){
      std::string encoded_string = encoded_byte_array;
      std::cout<<encoded_string << '\n';
      curl_free(encoded_byte_array);
    }
    curl_easy_cleanup(curl);
  }
  return encoded_string;
}

