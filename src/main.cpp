#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "Metainfo.h"
#include "MetainfoHandler.h"

int main(){

  MetainfoHandler m{};
  std::string path{"/home/kraffchenko/Downloads/debian-13.1.0-amd64-netinst.iso.torrent"};
  Metainfo info {m.createMetainfo(path)};
  std::cout << info.getPieceLength() << '\n';
  //std::cout << info.getPieces() << '\n';
  std::cout << info.getName() << '\n';
  std::cout << info.getLength() << '\n';

  return 0;

}
