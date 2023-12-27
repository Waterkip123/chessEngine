#include <iostream>
#include <sstream>
#include <vector>

#include <map>
#include <algorithm>
#include <random>

#include "ChessBoard.hpp"

Vec2::Vec2() {}
Vec2::Vec2(int x, int y) : x(x), y(y) {}

Vec2 Vec2::operator+(Vec2 rhs)
{
    Vec2 result = Vec2(x + rhs.x, y + rhs.y);
    return result;
}

Vec2 Vec2::operator*(int rhs)
{
    Vec2 result = Vec2(x * rhs, y * rhs);
    return result;
}

bool Vec2::operator==(Vec2 rhs)
{
    bool result = (x == rhs.x) && (y == rhs.y);
    return result;
}

std::map<PieceType, std::vector<Vec2>> directionsMap =
    {{PieceType::WhitePawn, std::vector<Vec2>{Vec2{0, 1}, Vec2{1, 1}, Vec2{-1, 1}}},
     {PieceType::BlackPawn, std::vector<Vec2>{Vec2{0, -1}, Vec2{1, -1}, Vec2{-1, -1}}},
     {PieceType::Knight, std::vector<Vec2>{Vec2{1, 2}, Vec2{2, 1}, Vec2{2, -1}, Vec2{1, -2}, Vec2{-1, -2}, Vec2{-2, -1}, Vec2{-2, 1}, Vec2{-1, 2}}},
     {PieceType::Bishop, std::vector<Vec2>{Vec2{1, 1}, Vec2{1, -1}, Vec2{-1, -1}, Vec2{-1, 1}}},
     {PieceType::Rook, std::vector<Vec2>{Vec2{0, 1}, Vec2{1, 0}, Vec2{0, -1}, Vec2{-1, 0}}},
     {PieceType::Queen, std::vector<Vec2>{Vec2{0, 1}, Vec2{1, 1}, Vec2{1, 0}, Vec2{1, -1}, Vec2{0, -1}, Vec2{-1, -1}, Vec2{-1, 0}, Vec2{-1, 1}}},
     {PieceType::King, std::vector<Vec2>{Vec2{0, 1}, Vec2{1, 1}, Vec2{1, 0}, Vec2{1, -1}, Vec2{0, -1}, Vec2{-1, -1}, Vec2{-1, 0}, Vec2{-1, 1}}}};

std::map<PieceColor, PieceColor> oppositeColorMap =
    {{PieceColor::White, PieceColor::Black},
     {PieceColor::Black, PieceColor::White}};

Piece::Piece(PieceType pieceType, PieceColor pieceColor, Vec2 position, bool singleStep) : pieceType(pieceType),
                                                                                           pieceColor(pieceColor), position(position), singleStep(singleStep) {}

std::vector<Vec2> &Piece::getDirections()
{
    return directions;
}

PieceList::PieceList(std::vector<Piece> &whitePieces, std::vector<Piece> &blackPieces) : whitePieces(whitePieces), blackPieces(blackPieces) {}

ChessMove::ChessMove() {}
ChessMove::ChessMove(Vec2 startSquare, Vec2 targetSquare) : startSquare(startSquare), targetSquare(targetSquare) {}

bool ChessMove::operator==(ChessMove rhs)
{
    bool result = (startSquare == rhs.startSquare) && (targetSquare == rhs.targetSquare);
    return result;
}

void ChessBoard::clean()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            // board[i][j] = nullptr;
        }
    }
}

void ChessGame::parseFEN(const std::string &fen)
{
    std::istringstream fenStream(fen);
    std::vector<std::string> tokens;

    // Split the FEN string into tokens
    std::string token;
    while (std::getline(fenStream, token, ' '))
    {
        tokens.push_back(token);
    }

    // Parse the board state
    int row = 0;
    int col = 0;
    for (char c : tokens[0])
    {

        if (c == '/')
        {
            row++;
            col = 0;
        }
        else if (isdigit(c))
        {
            // for (int i = 0; i < c - '0'; i++)
            //{
            //     position.board[row][col + i] = '0';
            // }
            col += c - '0';
        }
        else
        {
            // position.board[row][col] = c;

            bool isUpper = isupper(c);
            std::vector<Piece> *pieceList = &blackPieces;
            PieceColor pieceColor = PieceColor::Black;

            if (isUpper)
            {
                pieceColor = PieceColor::White;
                c += 32;
                pieceList = &whitePieces;
            }

            switch (c)
            {
            case 'p':
                if (isUpper)
                {
                    pieceList->push_back(Piece(PieceType::WhitePawn, pieceColor, Vec2(col, 7 - row), true));
                }
                else
                {
                    pieceList->push_back(Piece(PieceType::BlackPawn, pieceColor, Vec2(col, 7 - row), true));
                }
                break;
            case 'n':
                pieceList->push_back(Piece(PieceType::Knight, pieceColor, Vec2(col, 7 - row), true));
                break;
            case 'b':
                pieceList->push_back(Piece(PieceType::Bishop, pieceColor, Vec2(col, 7 - row), false));
                break;
            case 'r':
                pieceList->push_back(Piece(PieceType::Rook, pieceColor, Vec2(col, 7 - row), false));
                break;
            case 'q':
                pieceList->push_back(Piece(PieceType::Queen, pieceColor, Vec2(col, 7 - row), false));
                break;
            case 'k':
                pieceList->push_back(Piece(PieceType::King, pieceColor, Vec2(col, 7 - row), true));
                break;
            default:
                break;
            }

            col++;
        }
    }

    // Parse other information
    /*position.activeColor = tokens[1][0];
    position.castlingRights = tokens[2];
    position.enPassantTarget = tokens[3];
    position.halfmoveClock = std::stoi(tokens[4]);
    position.fullmoveNumber = std::stoi(tokens[5]);*/
}

PieceList ChessGame::getPieceList()
{
    return PieceList(whitePieces, blackPieces);
}

void ChessGame::calcMoves()
{
    generateAllMoves();
}

void ChessGame::makeMove(ChessMove move)
{
    if (isOccupied(move.targetSquare, PieceColor::White) || isOccupied(move.targetSquare, PieceColor::Black))
    {
        capturePiece(move.targetSquare);
    }

    for (Piece &piece : whitePieces)
    {
        if (piece.position == move.startSquare)
        {
            piece.position = move.targetSquare;
            piece.hasMoved = true;
            // drawRect(dc, move.targetSquare, wxColour(0, 200, 0));
        }
    }

    for (Piece &piece : blackPieces)
    {
        if (piece.position == move.startSquare)
        {
            piece.position = move.targetSquare;
            piece.hasMoved = true;

            // drawRect(dc, move.targetSquare, wxColour(0, 200, 0));
        }
    }
}

void ChessGame::generateMoves(Piece &piece, std::vector<ChessMove> &moves)
{
    std::vector<int8_t> distances(4);
    // piece.getDistanceToEdges(distances);
    std::vector<Vec2> directions = directionsMap[piece.pieceType];
    // std::vector<ChessMove> moves;

    // drawRect(dc, piece.position, wxColour(0, 0, 200));

    if (piece.singleStep)
    {
        if (piece.pieceType == PieceType::WhitePawn || piece.pieceType == PieceType::BlackPawn)
        {
            Vec2 tempMove;

            if (!piece.hasMoved)
            {
                tempMove = piece.position + directions[0] * 2;
                if (tempMove.x <= 7 && tempMove.x >= 0 && tempMove.y <= 7 && tempMove.y >= 0 && !isOccupied(tempMove, piece.pieceColor))
                {
                    ChessMove move(piece.position, tempMove);
                    moves.push_back(move);
                }
            }

            tempMove = piece.position + directions[1];
            if (tempMove.x <= 7 && tempMove.x >= 0 && tempMove.y <= 7 && tempMove.y >= 0 && isOccupied(tempMove, oppositeColorMap[piece.pieceColor]))
            {
                ChessMove move(piece.position, tempMove);
                moves.push_back(move);
            }

            tempMove = piece.position + directions[2];
            if (tempMove.x <= 7 && tempMove.x >= 0 && tempMove.y <= 7 && tempMove.y >= 0 && isOccupied(tempMove, oppositeColorMap[piece.pieceColor]))
            {
                ChessMove move(piece.position, tempMove);
                moves.push_back(move);
            }

            tempMove = piece.position + directions[0];
            if (tempMove.x <= 7 && tempMove.x >= 0 && tempMove.y <= 7 && tempMove.y >= 0 && !isOccupied(tempMove, piece.pieceColor) && !isOccupied(tempMove, oppositeColorMap[piece.pieceColor]))
            {
                ChessMove move(piece.position, tempMove);
                moves.push_back(move);
            }
        }
        else
        {
            for (Vec2 dir : directions)
            {
                Vec2 tempMove = piece.position + dir;
                if (tempMove.x <= 7 && tempMove.x >= 0 && tempMove.y <= 7 && tempMove.y >= 0 && !isOccupied(tempMove, piece.pieceColor))
                {
                    ChessMove move(piece.position, tempMove);
                    moves.push_back(move);
                }
            }
        }
    }
    else
    {

        for (Vec2 dir : directions)
        {

            bool validMove = true;
            int n = 1;
            while (validMove)
            {
                Vec2 tempMove = piece.position + dir * n;
                if (tempMove.x <= 7 && tempMove.x >= 0 && tempMove.y <= 7 && tempMove.y >= 0 && !isOccupied(tempMove, piece.pieceColor))
                {
                    ChessMove move(piece.position, tempMove);
                    // drawRect(dc, move.targetSquare, wxColour(200, 0, 0));
                    moves.push_back(move);
                    // makeMove(move, dc);
                }
                else
                {
                    validMove = false;
                }

                n++;
            }
        }
    }
}

// bool isLegal(int dir, int square, int distance) {}
void ChessGame::generateAllMoves()
{
    std::vector<ChessMove> whiteMoves;
    std::vector<ChessMove> blackMoves;

    if (depth > 0)
    {
        return;
    }
    depth += 1;

    for (Piece piece : whitePieces)
    {
        generateMoves(piece, whiteMoves);
        // makeMove(whiteMoves[0]);
    }

    for (Piece piece : blackPieces)
    {
        generateMoves(piece, blackMoves);
        // makeMove(blackMoves[0]);
    }

    if (whiteMoves.size() != 0)
    {
        // makeMove(whiteMoves[0]);
        // generateAllMoves();
    }

    if (blackMoves.size() != 0)
    {
        // makeMove(blackMoves[0]);
        // generateAllMoves();
    }
}

bool ChessGame::isOccupied(Vec2 position, PieceColor pieceColor)
{
    bool result = false;

    if (pieceColor == PieceColor::White)
    {
        for (Piece piece : whitePieces)
        {
            if (piece.position == position)
            {
                result = true;
            }
        }
    }
    else
    {
        for (Piece piece : blackPieces)
        {
            if (piece.position == position)
            {
                result = true;
            }
        }
    }

    return result;
}

void ChessGame::capturePiece(Vec2 position)
{
    for (int i = 0; i < whitePieces.size(); i++)
    {
        if (whitePieces[i].position == position)
        {
            whitePieces.erase(whitePieces.begin() + i);
        }
    }

    for (int i = 0; i < blackPieces.size(); i++)
    {
        if (blackPieces[i].position == position)
        {
            blackPieces.erase(blackPieces.begin() + i);
        }
    }
}

bool ChessGame::isLegal()
{
    return false;
}
