#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <array>

#include "Piece.hpp"

// struct PieceList
// {
//     PieceList(std::vector<Piece> &whitePieces, std::vector<Piece> &blackPieces);

//     std::vector<Piece> &whitePieces;
//     std::vector<Piece> &blackPieces;
// };

class ChessBoard
{
public:
    ChessBoard();
    void update(std::vector<Piece> &whitePieces, std::vector<Piece> &blackPieces);
    void clean();

    std::array<std::array<Piece *, 8>, 8> board;
};
