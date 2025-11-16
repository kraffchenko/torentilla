#ifndef SESSION_H
#define SESSION_H

#include<array>
#include <iostream>
#include <random>
#include <array>
#include <iostream>

class Session{
public:
  Session();
  std::array<std::byte, 20> const getPeerID(){ return m_peer_id;};
private:
  static std::array<std::byte, 20> generateID();
  std::array<std::byte, 20>m_peer_id {};
};


#endif
