#include "Session.h"

namespace{
}
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
void Session::downloadTorrent(const std::string_view dottorrent_path,
                              const std::string_view path_to_install,
                              const std::string_view filename_to_install){
  boost::asio::io_context cntx{};
  torrent::dottorrent::Metadata metadata {torrent::dottorrent::fromDotTorrent(dottorrent_path)};
  torrent::ResumeFile resume_file{reinterpret_cast<const char*>(metadata.getInfoHash().data()), 
                      filename_to_install, path_to_install, static_cast<size_t>(metadata.getLength()),
                      static_cast<size_t>(metadata.getPieceLength())}; 
  torrent::File download_file{path_to_install, filename_to_install, resume_file, metadata};
  torrent::LocalPeer local_peer{cntx};
  local_peer.acceptConnection();
  cntx.run();
}
void Session::createDotTorrent(torrent::dottorrent::Config config){
  if (!torrent::dottorrent::createDotTorrent(config)){
    std::cout << "Error while creating a .torrent." << '\n';
  }else{
    std::cout << ".torrent file was created.";
  }
}

