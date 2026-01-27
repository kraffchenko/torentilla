#include "net/CommunicationManager.h"
namespace net{
  bool CommunicationManager::connectionExists(net::Connection& connection){  
    if(m_connections.contains(connection.getSocket().remote_endpoint().address().to_string())){
      return true;
    }else{
      return false;
    }
  }
  void CommunicationManager::addConnection(net::Connection&& connection){
   if(!connectionExists(connection)){
      m_connections.insert({connection.getSocket().remote_endpoint().address().to_string(), std::move(connection)});
    } 
  }
  int CommunicationManager::addPendingConnection(net::Connection&& connection){
    int pending_amount {static_cast<int>(m_pending_connections.size())};
    m_pending_connections.insert({pending_amount, std::move(connection)});
    return pending_amount; 
  }
  net::Connection& net::CommunicationManager::getPendingConnection(int index){
    if(m_pending_connections.contains(index)){
      return m_pending_connections.at(index);
    }
  }
  net::Connection& net::CommunicationManager::getConnection(std::string& ip){
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
