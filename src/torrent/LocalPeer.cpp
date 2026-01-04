#include "torrent/LocalPeer.h"
namespace torrent{
  LocalPeer::LocalPeer(boost::asio::io_context& io_context)
  : m_acceptor{io_context}
  {
  } 
  void LocalPeer::connect(torrent::Peer& remote_peer){
    std::cout << "Trying to connect to " << remote_peer.getIp() << "..." << '\n';
    boost::asio::async_connect(remote_peer.getConnection().getSocket(), 
                  std::views::single(remote_peer.getEndpoint()), 
                  std::bind(&LocalPeer::handleOutConnection, this, std::ref(remote_peer), boost::asio::placeholders::error)); // std::views::single imitates a range for a single element
  };
  void LocalPeer::acceptConnection(){
    std::cout << "Accepting connections..." << '\n';
    
    torrent::Connection connection {torrent::Connection{m_acceptor.get_executor()}}; // 
    m_acceptor.async_accept(connection.getSocket(), 
                            [this, conn = std::move(connection)](const boost::system::error_code& error) mutable {
      handleInConnection(std::move(conn), error);
    }
      ); // fills the empty Connection object (the created empty socket) with incoming connection, calls a handler function which is esentially a separate function that just does extra stuff and recursively calls acceptConnection to accept new connections.
  };
  void LocalPeer::handleInConnection(torrent::Connection connection, 
                                   const boost::system::error_code& error){
    if(!error){
      boost::asio::ip::tcp::socket& connection_socket {connection.getSocket()};
      boost::asio::ip::tcp::endpoint remote_endpoint{connection_socket.remote_endpoint()};
      std::string remote_ip{remote_endpoint.address().to_string()};
      int port{static_cast<int>(remote_endpoint.port())};
      if(m_connections.count(remote_ip) < 0){
        std::cout << "Accepted connection from " << remote_ip << " is closed due to redundancy." << '\n';
        connection_socket.close(); 
      }else{
        m_connections.insert({remote_ip, torrent::Peer{remote_ip, port, std::move(connection)}});
        std::cout << "Connection request from " << remote_ip << " was accepted." << '\n';
        std::cout << "Connection successfully established.";
      }
    }
    acceptConnection();
  };
  void LocalPeer::handleOutConnection(torrent::Peer& remote_peer, const boost::system::error_code& err){
    if(!err){
      boost::asio::ip::tcp::endpoint remote_endpoint{remote_peer.getEndpoint()};
      std::string remote_ip{remote_endpoint.address().to_string()};
      int port{static_cast<int>(remote_endpoint.port())};
      if(m_connections.count(remote_ip) < 0){
        remote_peer.getConnection().getSocket().close(); 
        std::cout << "Established connection to " << remote_ip << " is closed due to redundancy." << '\n';
      }else{
        std::cout << "Successfully connected to " << remote_ip << "." << '\n';
        m_connections.insert({remote_ip, std::move(remote_peer)});
      }
    }else{
      std::cout << err.what() << '\n';
    }
  }
}
