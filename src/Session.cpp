#include "Session.h"

Session::Session()
: m_peer_id{generateID()}
{
};

std::array<std::byte, 20> Session::generateID(){

  std::mt19937 mt{std::random_device{}()};
  std::array<std::byte, 20> peer_id{std::byte{45}, std::byte{84}, std::byte{76}, std::byte{48}, std::byte{48}, std::byte{48}, std::byte{49}, std::byte{45}}; //creates an array with client signature
   

  std::uniform_int_distribution ascii_numbers{48, 57};

  for(int i{8}; i < 20; ++i){
    peer_id[i] = static_cast<std::byte>(ascii_numbers(mt));
  }

  return peer_id;
}
void Session::downloadTorrent(std::string& path){
  metainfo::Data info {metainfo::fromDotTorrent(path)};
}
void Session::createDotTorrent(torrent::Config config){
  if (!torrent::createDotTorrent(config)){
    std::cout << "Error while creating a .torrent." << '\n';
  }else{
    std::cout << ".torrent file was created.";
  }
}
