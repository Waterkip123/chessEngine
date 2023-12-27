#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <array>

struct Vec2
{
    Vec2();
    Vec2(int x, int y);

    int8_t x;
    int8_t y;

    Vec2 operator+(Vec2 rhs);
    Vec2 operator*(int rhs);
    bool operator==(Vec2 rhs);
};

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

struct ChessMove
{
    ChessMove();
    ChessMove(Vec2 startSquare, Vec2 targetSquare);

    Vec2 startSquare;
    Vec2 targetSquare;

    bool operator==(ChessMove rhs);
};

class Piece
{
public:
    Piece(PieceType pieceType, PieceColor pieceColor, Vec2 position, bool singleStep);

    PieceType pieceType;
    PieceColor pieceColor;
    Vec2 position;
    bool singleStep;
    bool hasMoved;

    std::vector<Vec2> directions;
    std::vector<Vec2> &getDirections();
};

struct PieceList
{
    PieceList(std::vector<Piece> &whitePieces, std::vector<Piece> &blackPieces);

    std::vector<Piece> &whitePieces;
    std::vector<Piece> &blackPieces;
};

class ChessBoard
{
public:
    ChessBoard();

    std::array<std::array<Piece, 8>, 8> board;

    void clean();
};

class ChessGame
{
public:
    int depth;

    std::vector<Piece> whitePieces;
    std::vector<Piece> blackPieces;

    void parseFEN(const std::string &fen);
    PieceList getPieceList();
    void calcMoves();
    void makeMove(ChessMove move);

private:
    void generateMoves(Piece &piece, std::vector<ChessMove> &moves);
    void generateAllMoves();
    bool isOccupied(Vec2 position, PieceColor pieceColor);
    void capturePiece(Vec2 position);
    bool isLegal();
};
