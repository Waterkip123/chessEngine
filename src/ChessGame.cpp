#include "ChessGame.hpp"

ChessGame::ChessGame() {}

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

    Piece *piece = board.board[move.startSquare.x][move.startSquare.y];

    piece->position = move.targetSquare;
    piece->hasMoved = true;

    if ((piece->pieceType == PieceType::WhitePawn || piece->pieceType == PieceType::BlackPawn) && move.targetSquare == enPassantTarget.first)
    {
        capturePiece(enPassantTarget.second->position);
    }

    int8_t moveX = move.targetSquare.x - move.startSquare.x;

    if (piece->pieceType == PieceType::King && moveX == 2)
    {
        ChessMove rookMove(Vec2(7, piece->position.y), Vec2(5, piece->position.y));

        makeMove(rookMove);
    }

    if (piece->pieceType == PieceType::King && moveX == -2)
    {
        ChessMove rookMove(Vec2(0, piece->position.y), Vec2(3, piece->position.y));

        makeMove(rookMove);
    }

    enPassantTarget = std::make_pair(Vec2(), nullptr);

    if (piece->pieceType == PieceType::WhitePawn && move.targetSquare.y - move.startSquare.y == 2)
    {

        enPassantTarget = std::make_pair(move.targetSquare + Vec2(0, -1), piece);
    }

    if (piece->pieceType == PieceType::BlackPawn && move.targetSquare.y - move.startSquare.y == -2)
    {
        enPassantTarget = std::make_pair(move.targetSquare + Vec2(0, 1), piece);
    }

    board.update(whitePieces, blackPieces);
}

void ChessGame::generateMoves(Piece &piece, std::vector<ChessMove> &moves)
{
    std::vector<int8_t> distances(4);
    // piece.getDistanceToEdges(distances);
    std::vector<Vec2> directions = directionsMap[piece.pieceType];
    // std::vector<ChessMove> moves;

    // drawRect(dc, piece.position, wxColour(0, 0, 200));

    switch (piece.pieceType)
    {
    case PieceType::WhitePawn:
        generatePawnMoves(piece, moves);
        break;
    case PieceType::BlackPawn:
        generatePawnMoves(piece, moves);
        break;
    case PieceType::Knight:
        generateKnightMoves(piece, moves);
        break;
    case PieceType::Bishop:
        generateBishopMoves(piece, moves);
        break;
    case PieceType::Rook:
        generateRookMoves(piece, moves);
        break;
    case PieceType::Queen:
        generateQueenMoves(piece, moves);
        break;
    case PieceType::King:
        generateKingMoves(piece, moves);
        break;
    default:
        break;
    }
}

void ChessGame::generatePawnMoves(Piece &piece, std::vector<ChessMove> &moves)
{
    Vec2 tempMove;
    std::vector<Vec2> directions = directionsMap[piece.pieceType];

    // double pawn move
    if (!piece.hasMoved)
    {
        tempMove = piece.position + directions[0] * 2;
        if (tempMove.x <= 7 && tempMove.x >= 0 && tempMove.y <= 7 && tempMove.y >= 0 && !isOccupied(tempMove, piece.pieceColor) && !isOccupied(tempMove, oppositeColorMap[piece.pieceColor]))
        {
            ChessMove move(piece.position, tempMove);
            moves.push_back(move);
        }
    }

    tempMove = piece.position + directions[1];
    if (tempMove.x <= 7 && tempMove.x >= 0 && tempMove.y <= 7 && tempMove.y >= 0 && (isOccupied(tempMove, oppositeColorMap[piece.pieceColor]) || tempMove == enPassantTarget.first))
    {
        ChessMove move(piece.position, tempMove);
        moves.push_back(move);
    }

    tempMove = piece.position + directions[2];
    if (tempMove.x <= 7 && tempMove.x >= 0 && tempMove.y <= 7 && tempMove.y >= 0 && (isOccupied(tempMove, oppositeColorMap[piece.pieceColor]) || tempMove == enPassantTarget.first))
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

void ChessGame::generateKnightMoves(Piece &piece, std::vector<ChessMove> &moves)
{
    std::vector<Vec2> directions = directionsMap[piece.pieceType];

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

void ChessGame::generateBishopMoves(Piece &piece, std::vector<ChessMove> &moves)
{
    std::vector<Vec2> directions = directionsMap[piece.pieceType];

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
                moves.push_back(move);

                if (isOccupied(tempMove, oppositeColorMap[piece.pieceColor]))
                {
                    validMove = false;
                }
            }
            else
            {
                validMove = false;
            }

            n++;
        }
    }
}

void ChessGame::generateRookMoves(Piece &piece, std::vector<ChessMove> &moves)
{
    std::vector<Vec2> directions = directionsMap[piece.pieceType];

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
                moves.push_back(move);

                if (isOccupied(tempMove, oppositeColorMap[piece.pieceColor]))
                {
                    validMove = false;
                }
            }
            else
            {
                validMove = false;
            }

            n++;
        }
    }
}

void ChessGame::generateQueenMoves(Piece &piece, std::vector<ChessMove> &moves)
{
    std::vector<Vec2> directions = directionsMap[piece.pieceType];

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
                moves.push_back(move);

                if (isOccupied(tempMove, oppositeColorMap[piece.pieceColor]))
                {
                    validMove = false;
                }
            }
            else
            {
                validMove = false;
            }

            n++;
        }
    }
}

void ChessGame::generateKingMoves(Piece &piece, std::vector<ChessMove> &moves)
{
    std::vector<Vec2> directions = directionsMap[piece.pieceType];

    for (Vec2 dir : directions)
    {
        Vec2 tempMove = piece.position + dir;
        if (tempMove.x <= 7 && tempMove.x >= 0 && tempMove.y <= 7 && tempMove.y >= 0 && !isOccupied(tempMove, piece.pieceColor))
        {
            ChessMove move(piece.position, tempMove);
            moves.push_back(move);
        }
    }

    int8_t kingY = piece.position.y;

    Piece *kingSideRook = board.board[7][kingY];
    Piece *queenSideRook = board.board[0][kingY];

    if (!piece.hasMoved && !kingSideRook->hasMoved && board.board[6][kingY] == nullptr && board.board[5][kingY] == nullptr)
    {
        Vec2 kingMove(6, kingY);
        moves.push_back(ChessMove(piece.position, kingMove));
    }

    if (!piece.hasMoved && !queenSideRook->hasMoved && board.board[1][kingY] == nullptr && board.board[2][kingY] == nullptr && board.board[3][kingY] == nullptr)
    {
        Vec2 kingMove(2, kingY);
        moves.push_back(ChessMove(piece.position, kingMove));
    }
}

void ChessGame::generateTargetedSquares()
{
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
    }

    for (Piece piece : blackPieces)
    {
        generateMoves(piece, blackMoves);
    }
}

bool ChessGame::isOccupied(Vec2 position, PieceColor pieceColor)
{
    bool result = false;

    if (board.board[position.x][position.y] != nullptr && board.board[position.x][position.y]->pieceColor == pieceColor)
    {
        result = true;
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

ChessMove::ChessMove() {}
ChessMove::ChessMove(Vec2 startSquare, Vec2 targetSquare) : startSquare(startSquare), targetSquare(targetSquare) {}

bool ChessMove::operator==(ChessMove rhs)
{
    bool result = (startSquare == rhs.startSquare) && (targetSquare == rhs.targetSquare);
    return result;
}
