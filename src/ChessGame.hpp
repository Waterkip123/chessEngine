#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>

#include "Utils.hpp"
#include "ChessBoard.hpp"
#include "Piece.hpp"

struct ChessMove
{
    ChessMove();
    ChessMove(Vec2 startSquare, Vec2 targetSquare);

    Vec2 startSquare;
    Vec2 targetSquare;

    bool operator==(ChessMove rhs);
};

class ChessGame
{
public:
    ChessGame();
    ChessBoard board;
    int depth;

    std::vector<Piece> whitePieces;
    std::vector<Piece> blackPieces;
    std::vector<Vec2> whiteTargetedSquares;
    std::vector<Vec2> blackTargetedSquares;

    std::pair<Vec2, Piece *> enPassantTarget;

    void parseFEN(const std::string &fen);
    // PieceList getPieceList();
    void calcMoves();
    void makeMove(ChessMove move);
    void generateMoves(Piece &piece, std::vector<ChessMove> &moves);

private:
    void generateAllMoves();
    bool isOccupied(Vec2 position, PieceColor pieceColor);
    void capturePiece(Vec2 position);
    bool isLegal();
    void generatePawnMoves(Piece &piece, std::vector<ChessMove> &moves);
    void generateKnightMoves(Piece &piece, std::vector<ChessMove> &moves);
    void generateBishopMoves(Piece &piece, std::vector<ChessMove> &moves);
    void generateRookMoves(Piece &piece, std::vector<ChessMove> &moves);
    void generateQueenMoves(Piece &piece, std::vector<ChessMove> &moves);
    void generateKingMoves(Piece &piece, std::vector<ChessMove> &moves);
    void generateTargetedSquares();
};

static std::map<PieceType, std::vector<Vec2>> directionsMap =
    {{PieceType::WhitePawn, std::vector<Vec2>{Vec2{0, 1}, Vec2{1, 1}, Vec2{-1, 1}}},
     {PieceType::BlackPawn, std::vector<Vec2>{Vec2{0, -1}, Vec2{1, -1}, Vec2{-1, -1}}},
     {PieceType::Knight, std::vector<Vec2>{Vec2{1, 2}, Vec2{2, 1}, Vec2{2, -1}, Vec2{1, -2}, Vec2{-1, -2}, Vec2{-2, -1}, Vec2{-2, 1}, Vec2{-1, 2}}},
     {PieceType::Bishop, std::vector<Vec2>{Vec2{1, 1}, Vec2{1, -1}, Vec2{-1, -1}, Vec2{-1, 1}}},
     {PieceType::Rook, std::vector<Vec2>{Vec2{0, 1}, Vec2{1, 0}, Vec2{0, -1}, Vec2{-1, 0}}},
     {PieceType::Queen, std::vector<Vec2>{Vec2{0, 1}, Vec2{1, 1}, Vec2{1, 0}, Vec2{1, -1}, Vec2{0, -1}, Vec2{-1, -1}, Vec2{-1, 0}, Vec2{-1, 1}}},
     {PieceType::King, std::vector<Vec2>{Vec2{0, 1}, Vec2{1, 1}, Vec2{1, 0}, Vec2{1, -1}, Vec2{0, -1}, Vec2{-1, -1}, Vec2{-1, 0}, Vec2{-1, 1}}}};

static std::map<PieceColor, PieceColor> oppositeColorMap =
    {{PieceColor::White, PieceColor::Black},
     {PieceColor::Black, PieceColor::White}};

// PieceList::PieceList(std::vector<Piece> &whitePieces, std::vector<Piece> &blackPieces) : whitePieces(whitePieces), blackPieces(blackPieces) {}
