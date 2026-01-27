#include "net/Connection.h"

namespace net{
   //void Connection::sendHandshake(std::array<std::byte, 20> info_hash, std::array<std::byte, 20> peer_id){
   //m_out_buffer.clear();

   // int pstrlen{19};
   // std::string pstr{"BitTorrent protocol"};
   // std::array<std::byte, 8> reserved;
   // reserved.fill(static_cast<std::byte>(0));

   // m_out_buffer.push_back(static_cast<std::byte>(pstrlen));
   // auto it_pstr {std::transform(pstr.begin(), pstr.end(), &m_out_buffer.back(), [](char ch){ return static_cast<std::byte>(ch); })};
   // auto it_reserved {std::copy(reserved.begin(), reserved.end(), it_pstr)};
   // auto it_info{std::copy(info_hash.begin(), info_hash.end(), it_reserved)};
   // std::copy(peer_id.begin(), peer_id.end(), it_info);
   // }
  Connection::Connection(boost::asio::io_context& io_context)
  : m_socket{io_context.get_executor()}
  {
    m_socket.open(boost::asio::ip::tcp::v4());
  }
  Connection::Connection(boost::asio::any_io_executor io_exec)
  : m_socket {io_exec}
  {
    m_socket.open(boost::asio::ip::tcp::v4());
  }
  boost::asio::ip::tcp::socket& Connection::getSocket(){
    return m_socket;
  }
  boost::asio::const_buffer Connection::createSubBuffer(std::vector<std::byte> message_as_ba){
    auto sub_buffer_start {this -> m_out_buffer.end()}; 
    this -> m_out_buffer.insert(sub_buffer_start, message_as_ba.begin(),
                                message_as_ba.end());
    int64_t sub_buffer_size{std::distance(sub_buffer_start, this -> m_out_buffer.end())};
    return boost::asio::const_buffer(&(*sub_buffer_start), static_cast<size_t>(sub_buffer_size));
  }

}
