#include "torrent/Download.h"
namespace torrent{
  Download::Download(metainfo::Data& dot_torrent_ref, bool compact=true, bool no_peer_id=true)
    : m_dot_torrent_ref {dot_torrent_ref}, m_compact{compact}, m_no_peer_id{no_peer_id}
    {
    }
}
