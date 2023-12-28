#include <iostream>
#include <sstream>
#include <vector>

#include <map>
#include <algorithm>
#include <random>

#include "ChessBoard.hpp"
#include "Utils.hpp"

ChessBoard::ChessBoard()
{
    clean();
}

void ChessBoard::clean()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i][j] = nullptr;
        }
    }
}

void ChessBoard::update(std::vector<Piece> &whitePieces, std::vector<Piece> &blackPieces)
{
    clean();

    for (Piece &piece : whitePieces)
    {
        board[piece.position.x][piece.position.y] = &piece;
    }

    for (Piece &piece : blackPieces)
    {
        board[piece.position.x][piece.position.y] = &piece;
    }
}