#include "torrent/Communication.h"

namespace torrent{
  void Communication::sendHandshake(std::array<std::byte, 20>& info_hash, std::array<std::byte, 20>& peer_id){
    int pstrlen{19};
    std::string pstr{"BitTorrent protocol"};
    std::bitset<8> reserved{0b00000000};
  }


}
