#ifndef TORRENTDOWNLOAD_H
#define TORRENTDOWNLOAD_H

#include <iostream>
#include <array>

#include "Metainfo.h"

class TorrentDownload{
public:
  TorrentDownload(Metainfo& dot_torrent_ref, bool compact, bool no_peer_id);
  std::string getPort(){return m_port;};
  int64_t getUploaded(){return m_uploaded;};
  int64_t getDownloaded(){return m_downloaded;};
  int64_t getLeft(){return m_left;};
  bool getCompact(){return m_compact;};
  bool getNoPeerId(){ return m_no_peer_id;};
  std::string getEvent(){return m_event;};
  std::array<std::byte, 20> getInfoHash(){return m_dot_torrent_ref.getInfoHash();};
  std::string_view getAnnounce(){return m_dot_torrent_ref.getAnnounce();};
private:
  Metainfo& m_dot_torrent_ref;
  std::string m_port{"6881"};
  int64_t m_uploaded{};
  int64_t m_downloaded{};
  int64_t m_left{};
  bool m_compact{true};
  bool m_no_peer_id{true};
  std::string m_event{"started"};
};

#endif
