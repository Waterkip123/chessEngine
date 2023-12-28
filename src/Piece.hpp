#pragma once

#include <vector>

#include "Utils.hpp"

enum class PieceType
{
    None,
    WhitePawn,
    BlackPawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

enum class PieceColor
{
    None,
    White,
    Black
};

class Piece
{
public:
    Piece(PieceType pieceType, PieceColor pieceColor, Vec2 position, bool singleStep);

    PieceType pieceType;
    PieceColor pieceColor;
    Vec2 position;
    bool singleStep;
    bool hasMoved = false;

    std::vector<Vec2> directions;
    std::vector<Vec2> &getDirections();
};