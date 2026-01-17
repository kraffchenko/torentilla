#include "torrent/LocalPeer.h"
namespace torrent{
  LocalPeer::LocalPeer(boost::asio::io_context& io_context)
  : m_acceptor{io_context}
  {
  } 
  void LocalPeer::connect(torrent::Peer& remote_peer, torrent::File& file, std::array<std::byte, 20>& peer_id){
    std::cout << "Trying to connect to " << remote_peer.getIp() << "..." << '\n';
    net::Connection connection {net::Connection{m_acceptor.get_executor()}};
    connection.getSocket().async_connect(
                            remote_peer.getEndpoint(),
                            [this, conn = std::move(connection), &peer_id, &file](const boost::system::error_code& error) mutable {
      handleOutConnection(std::move(conn), peer_id, file, error);
    });
  };
  void LocalPeer::acceptConnection(torrent::File& file, std::array<std::byte, 20>& peer_id){
    std::cout << "Accepting connections..." << '\n';
    net::Connection connection {net::Connection{m_acceptor.get_executor()}}; // 
    m_acceptor.async_accept(connection.getSocket(), 
                            [this, conn = std::move(connection), &file, &peer_id](const boost::system::error_code& error) mutable {
      handleInConnection(std::move(conn), peer_id, file, error);
    }
      ); // fills the empty Connection object (the created empty socket) with incoming connection, calls a handler function which is esentially a separate function that just does extra stuff and recursively calls acceptConnection to accept new connections.
  };
  void LocalPeer::handleInConnection(net::Connection connection,
                                     std::array<std::byte, 20>& peer_id,
                                     torrent::File& file,
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
        m_connections.insert({remote_ip, std::move(connection)});
        m_tcp_reader.read(connection, peer_id, file);
        std::cout << "Connection request from " << remote_ip << " was accepted." << '\n';
        std::cout << "Connection successfully established.";
      }
    }
    acceptConnection(file, peer_id);
  };
  void LocalPeer::handleOutConnection(net::Connection connection, std::array<std::byte, 20>& peer_id, 
                                      torrent::File& file, const boost::system::error_code& err){
    if(!err){
      boost::asio::ip::tcp::endpoint remote_endpoint{connection
                                                     .getSocket()
                                                     .remote_endpoint()};
      std::string remote_ip{remote_endpoint.address().to_string()};
      int port{static_cast<int>(remote_endpoint.port())};
      if(m_connections.count(remote_ip) < 0){
        connection.getSocket().close(); 
        std::cout << "Established connection to " << remote_ip << " is closed due to redundancy." << '\n';
      }else{
        std::cout << "Successfully connected to " << remote_ip << "." << '\n';
        m_connections.insert({remote_ip, std::move(connection)});
        m_tcp_reader.read(connection, peer_id, file);
      }
    }else{
      std::cout << err.what() << '\n';
    }
  }
}
