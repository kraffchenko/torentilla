#include "net/communication/CommunicationManager.h"
namespace net::communication{
  bool CommunicationManager::connectionExists(std::string& ip){  
    if(m_connections.contains(ip)){
      return true;
    }else{
      return false;
    }
  }
  void CommunicationManager::addConnection(net::Connection& connection){
    m_connections.insert({connection.getSocket().remote_endpoint().address().to_string(), connection}); 
  }
  net::Connection& CommunicationManager::getConnection(std::string& ip){
    if(m_connections.contains(ip)){
      return m_connections.at(ip);
    }
  }
  void CommunicationManager::closeConnection(net::Connection& connection){
    m_connections.at(connection.getSocket().remote_endpoint().address().to_string()).getSocket().close();
    m_connections.erase(connection.getSocket().remote_endpoint().address().to_string());
  }
}
