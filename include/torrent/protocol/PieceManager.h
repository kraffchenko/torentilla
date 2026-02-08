#ifndef PIECEMANAGER_H
#define PIECEMANAGER_H
#include <iostream>
#include <map>
#include "net/Connection.h"
#include "torrent/protocol/Piece.h"
namespace torrent::protocol{
  class PieceManager{
  public:
    PieceManager(size_t file_size, size_t piece_size);
    std::vector<std::reference_wrapper<net::Connection>>& getConnectionsByIndex(int index);
    bool areConnectionsWithPiece(size_t index);
    void addConnection(size_t index, net::Connection&);
    void initializePieces(size_t file_size, size_t piece_size);
    Piece& getPiece(size_t index);
  private:
    std::vector<torrent::protocol::Piece> m_all_pieces{};
    std::vector<std::vector<std::reference_wrapper<net::Connection>>> m_connections{};
  };
}
#endif
