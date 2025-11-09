#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "Metainfo.h"
#include "MetainfoHandler.h"
#include <curl/curl.h>

int main(){
  
  MetainfoHandler m{};
  std::string path{"/home/kraffchenko/Downloads/debian-13.1.0-amd64-netinst.iso.torrent"};
  Metainfo info {m.createMetainfo(path)};
  



  return 0;

}
