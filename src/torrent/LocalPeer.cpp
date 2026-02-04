#include "torrent/LocalPeer.h"
namespace torrent{
  LocalPeer::LocalPeer(boost::asio::io_context& io_context, net::CommunicationManager& com_manager)
  : m_acceptor{io_context}, m_com_manager{com_manager}
  {
  } 
  void LocalPeer::connect(torrent::Peer& remote_peer, torrent::File& file, std::array<std::byte, 20>& peer_id){
    std::cout << "Trying to connect to " << remote_peer.getIp() << "..." << '\n';
    net::Connection connection {net::Connection{m_acceptor.get_executor()}};
    std::string ip {remote_peer.getIp()};
    if(!m_com_manager.connectionExists(ip)){
      m_com_manager.addConnection(ip, std::move(connection));
      std::string ip{remote_peer.getIp()};
      m_com_manager.getConnection(ip).getSocket().async_connect(
                            remote_peer.getEndpoint(),
                            [this, &con = m_com_manager.getConnection(ip), &peer_id, &file](const boost::system::error_code& error) mutable {

      handleOutConnection(con, peer_id, file, error);
      });
    }else{
      return;
    }
  };
  void LocalPeer::acceptConnection(torrent::File& file, std::array<std::byte, 20>& peer_id){
    std::cout << "Accepting connections..." << '\n';
    net::Connection connection {net::Connection{m_acceptor.get_executor()}}; 
    int index {m_com_manager.addPendingConnection(std::move(connection))};
    m_acceptor.async_accept(m_com_manager.getPendingConnection(index).getSocket(), 
                            [this, &con = m_com_manager.getPendingConnection(index), index, &file, &peer_id](const boost::system::error_code& error) mutable {
      handleInConnection(con, index, peer_id, file, error);
    }
      ); // fills the empty Connection object (the created empty socket) with incoming connection, calls a handler function which is esentially a separate function that just does extra stuff and recursively calls acceptConnection to accept new connections.
  };
  void LocalPeer::handleInConnection(net::Connection& connection,
                                     int index,
                                     std::array<std::byte, 20>& peer_id,
                                     torrent::File& file,
                                     const boost::system::error_code& error){
    if(!error){
      boost::asio::ip::tcp::socket& connection_socket {connection.getSocket()};
      boost::asio::ip::tcp::endpoint remote_endpoint{connection_socket.remote_endpoint()};
      std::string remote_ip{remote_endpoint.address().to_string()};
      int port{static_cast<int>(remote_endpoint.port())};
      if(m_com_manager.connectionExists(remote_ip)){
        std::cout << "Accepted connection from " << remote_ip << " is closed due to redundancy." << '\n';
        m_com_manager.closePendingConnection(index);
      }else{
        std::cout << "Connection request from " << remote_ip << " was accepted." << '\n';
        std::cout << "Connection successfully established.";
        m_com_manager.addConnection(std::move(connection));
        net::tcp::read(connection, m_com_manager);
        net::tcp::sendHandshake(connection, m_com_manager);
      }
    }else{
      std::cout << error.what() << '\n';
    }
    acceptConnection(file, peer_id);
  };
  void LocalPeer::handleOutConnection(net::Connection& connection, std::array<std::byte, 20>& peer_id, 
                                      torrent::File& file, const boost::system::error_code& err){
    if(!err){
      boost::asio::ip::tcp::endpoint remote_endpoint{connection
                                                     .getSocket()
                                                     .remote_endpoint()};
      std::string remote_ip{remote_endpoint.address().to_string()};
      std::cout << "Successfully connected to " << remote_ip << "." << '\n';
      net::tcp::read(connection, m_com_manager);
      net::tcp::sendHandshake(connection, m_com_manager);
    }else{
      std::cout << err.value() << '\n';
    }
  }
}
