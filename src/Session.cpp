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
awaitable<void> Session::startDownloadingProcess(CommunicationManager& com_manager, PieceManager& piece_manager){

  //net::Connection out_con {co_await local_peer.connect(remote_peer)};
  //co_spawn(cntx, [&cntx, &local_peer, &com_manager, &piece_manager]() -> awaitable<void>{
    //for(;;){
   //   net::Connection in_con {co_await local_peer.acceptConnection()};
    //  co_spawn(cntx, net::communication::startCommunication(std::move(in_con), com_manager, piece_manager), detached);
   // }
  //}, detached);
  //co_spawn(cntx, net::communication::startCommunication(std::move(out_con), com_manager, piece_manager), detached);
  //cntx.run();
  co_return;
};
awaitable<void> Session::downloadTorrent(boost::asio::io_context& cntx, const std::string_view dottorrent_path,
                              const std::string_view path_to_install){

  torrent::dottorrent::Metadata metadata {torrent::dottorrent::fromDotTorrent(dottorrent_path)};
  std::string event{"started"};
  tracker::Request request{metadata, 6881, 0, 0, 0, true, false, event};
  request.sendGetRequest(m_peer_id);
  torrent::ResumeFile resume_file{ 
    !std::filesystem::exists(torrent::ResumeFile::pathBasedOnHash(metadata.getInfoHashAsString()))
    ? torrent::ResumeFile{metadata.getInfoHashAsString(), metadata.getName(), path_to_install,
                          static_cast<size_t>(metadata.getLength()),
                          (static_cast<size_t>(metadata.getLength()) / static_cast<size_t>(metadata.getPieceLength()))}
    : torrent::ResumeFile{torrent::ResumeFile::fromFile(
          torrent::ResumeFile::pathBasedOnHash(metadata.getInfoHashAsString()))}};
  torrent::File download_file{path_to_install, metadata.getName(), resume_file, metadata};
  CommunicationManager com_manager{download_file, m_peer_id};
  torrent::protocol::PieceManager piece_manager{static_cast<size_t>(download_file.m_metadata.getLength()),
                                                static_cast<size_t>(download_file.m_metadata.getPieceLength())};
  torrent::LocalPeer local_peer{cntx};
  std::string ip {"192.168.122.253"};
  torrent::Peer remote_peer{ip, 6881};
  net::Connection out_con{co_await local_peer.connect(remote_peer)};
  co_spawn(cntx, startCommunication(std::move(out_con), com_manager, piece_manager), detached);
  co_spawn(cntx, scheduleDownloadProcess(cntx, piece_manager, com_manager), detached);
  for(;;){
    net::Connection in_con {co_await local_peer.acceptConnection()};
    co_spawn(cntx, startCommunication(std::move(in_con), com_manager, piece_manager), detached);
  }
}
void Session::createDotTorrent(torrent::dottorrent::Config config){
  if (!torrent::dottorrent::createDotTorrent(config)){
    std::cout << "Error while creating a .torrent." << '\n';
  }else{
    std::cout << ".torrent file was created.";
  }
}

