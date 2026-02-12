#include "net/communication/CommunicationManager.h"
namespace net::communication{
  bool CommunicationManager::connectionExists(std::string& ip){  
    if(m_connections.contains(ip)){
      return true;
    }else{
      return false;
    }
  }
  void CommunicationManager::addConnection(net::Connection&& connection){
    m_connections.insert({connection.getSocket().remote_endpoint().address().to_string(), std::move(connection)}); 
  }
  void CommunicationManager::addConnection(std::string& ip, net::Connection&& connection){
    m_connections.insert({ip, std::move(connection)}); 
  }
  int CommunicationManager::addPendingConnection(net::Connection&& connection){
    int pending_amount {static_cast<int>(m_pending_connections.size())};
    m_pending_connections.insert({pending_amount, std::move(connection)});
    return pending_amount; 
  }
  net::Connection& CommunicationManager::getPendingConnection(int index){
    if(m_pending_connections.contains(index)){
      return m_pending_connections.at(index);
    }
  }
  net::Connection& CommunicationManager::getConnection(std::string& ip){
    if(m_connections.contains(ip)){
      return m_connections.at(ip);
    }
  }
  void CommunicationManager::closePendingConnection(int index){
    m_pending_connections.at(index).getSocket().close();
    m_pending_connections.erase(index);
  };
  void CommunicationManager::closeConnection(net::Connection& connection){
    m_connections.at(connection.getSocket().remote_endpoint().address().to_string()).getSocket().close();
    m_connections.erase(connection.getSocket().remote_endpoint().address().to_string());
  }
}
