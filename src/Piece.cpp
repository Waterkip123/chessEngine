#include "Piece.hpp"

Piece::Piece(PieceType pieceType, PieceColor pieceColor, Vec2 position, bool singleStep) : pieceType(pieceType),
                                                                                           pieceColor(pieceColor), position(position), singleStep(singleStep) {}

std::vector<Vec2> &Piece::getDirections()
{
    return directions;
}