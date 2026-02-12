#include "torrent/LocalPeer.h"
namespace torrent{
  LocalPeer::LocalPeer(boost::asio::io_context& io_context)
  : m_acceptor{io_context} 
  {
  } 
  awaitable<net::Connection> LocalPeer::connect(torrent::Peer& remote_peer){
    net::Connection connection {net::Connection{m_acceptor.get_executor()}};
    std::string ip {remote_peer.getIp()};
    co_await connection.getSocket().async_connect(remote_peer.getEndpoint(), use_awaitable);
    boost::asio::ip::tcp::endpoint remote_endpoint{connection.getSocket()
                                                  .remote_endpoint()};
    std::string remote_ip{remote_endpoint.address().to_string()};
    std::cout << "Successfully connected to " << remote_ip << "." << '\n';
    co_return connection;
  };
}
