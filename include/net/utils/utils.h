#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <array>
#include <vector>
#include <cmath>

#include <curl/curl.h>
#include <boost/compute/detail/sha1.hpp>  
#include <openssl/evp.h>
#include <boost/asio.hpp>

namespace net::utils{
 


  inline std::string url_encode(std::array<std::byte, 20> byte_array){
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
  inline std::array<std::byte, 20> returnSHA1(const std::vector<std::byte>& data){

    if (std::size(data) <= 0){
      throw std::invalid_argument("Invalid argument");
    }
    std::array<std::byte, 20> info_hash{};


    EVP_MD_CTX* mdctx{EVP_MD_CTX_create()}; //creates a pointer to the type EVP_MD_CTX(digest context type) by using EVP_MD_CTX_create() which allocates, initializes and returns digest context
    const EVP_MD *md {EVP_sha1()}; //initializes a variable which holds md(message digest=algorith to use) 
    unsigned int md_len {};
    EVP_DigestInit_ex(mdctx, md, NULL); //setting up the digest context with supplying it with necessary values/variables
    EVP_DigestUpdate(mdctx, data.data(), std::size(data)); //hashes the value of raw_bytes into digest context
    EVP_DigestFinal_ex(mdctx, reinterpret_cast<unsigned char*>(info_hash.data()), &md_len); //places the value of the digest context from the previous step and saves it into info_hash 
    EVP_MD_CTX_destroy(mdctx);

    return info_hash;
  }
  inline boost::asio::ip::tcp::endpoint createBoostEndpoint(std::string& ip, int port){
    return boost::asio::ip::tcp::endpoint{boost::asio::ip::make_address(ip), static_cast<uint16_t>(port)};
  } 

}
#endif
