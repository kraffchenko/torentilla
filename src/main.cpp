#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "Metainfo.h"

int main(){

  Metainfo m{};
  m.readTorrentFile("/home/kraffchenko/Downloads/debian-13.1.0-amd64-netinst.iso.torrent");
  return 0;

}
