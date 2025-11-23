#include "TorrentDownload.h"

TorrentDownload::TorrentDownload(Metainfo& dot_torrent_ref, bool compact=true, bool no_peer_id=true)
  : m_dot_torrent_ref {dot_torrent_ref}, m_compact{compact}, m_no_peer_id{no_peer_id}
  {
  }

