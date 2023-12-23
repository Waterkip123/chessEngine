#include <iostream>
#include <sstream>
#include <vector>

#include <map>

#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/cursor.h>
#include <wx/dragimag.h>

struct ChessPosition
{
    char board[8][8];
    char activeColor; // 'w' for white, 'b' for black
    std::string castlingRights;
    std::string enPassantTarget;
    int halfmoveClock;
    int fullmoveNumber;
};

struct Vec2
{
    Vec2(int x, int y) : x(x), y(y) {}
    int8_t x;
    int8_t y;

    Vec2 operator+(Vec2 rhs)
    {
        Vec2 result = Vec2(x + rhs.x, y + rhs.y);
        return result;
    }
};

enum class PieceType
{
    None,
    Pawn,
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

std::map<PieceType, std::vector<Vec2>> directionsMap =
    {{PieceType::Pawn, std::vector<Vec2>{Vec2{1, 0}, Vec2{1, 1}, Vec2{-1, 1}}},
     {PieceType::Knight, std::vector<Vec2>{Vec2{1, 2}, Vec2{2, 1}, Vec2{2, -1}, Vec2{1, -2}, Vec2{-1, -2}, Vec2{-2, -1}, Vec2{-2, 1}, Vec2{-1, 2}}},
     {PieceType::Bishop, std::vector<Vec2>{Vec2{1, 1}, Vec2{1, -1}, Vec2{-1, -1}, Vec2{-1, 1}}},
     {PieceType::Rook, std::vector<Vec2>{Vec2{0, 1}, Vec2{1, 0}, Vec2{0, -1}, Vec2{-1, 0}}},
     {PieceType::Queen, std::vector<Vec2>{Vec2{0, 1}, Vec2{1, 1}, Vec2{1, 0}, Vec2{1, -1}, Vec2{0, -1}, Vec2{-1, -1}, Vec2{-1, 0}, Vec2{-1, 1}}},
     {PieceType::King, std::vector<Vec2>{Vec2{0, 1}, Vec2{1, 1}, Vec2{1, 0}, Vec2{1, -1}, Vec2{0, -1}, Vec2{-1, -1}, Vec2{-1, 0}, Vec2{-1, 1}}}};

class Piece
{
public:
    Piece(PieceType pieceType, PieceColor pieceColor, Vec2 position, bool singleStep) : pieceType(pieceType),
                                                                                        pieceColor(pieceColor), position(position), singleStep(singleStep)
    {
    }

    PieceType pieceType;
    PieceColor pieceColor;
    Vec2 position{0, 0};
    bool singleStep;

    std::vector<Vec2> directions;

    std::vector<Vec2> &getDirections()
    {
        return directions;
    }
};

class Board
{
public:
    ChessPosition parseFEN(const std::string &fen)
    {
        ChessPosition position;

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
                for (int i = 0; i < c - '0'; i++)
                {
                    position.board[row][col + i] = '0';
                }
                col += c - '0';
            }
            else
            {
                position.board[row][col] = c;

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
                    pieceList->push_back(Piece(PieceType::Pawn, pieceColor, Vec2(col - 1, row - 1), true));
                    break;
                case 'n':
                    pieceList->push_back(Piece(PieceType::Knight, pieceColor, Vec2(col - 1, row - 1), true));
                    break;
                case 'b':
                    pieceList->push_back(Piece(PieceType::Bishop, pieceColor, Vec2(col - 1, row - 1), false));
                    break;
                case 'r':
                    pieceList->push_back(Piece(PieceType::Rook, pieceColor, Vec2(col - 1, row - 1), false));
                    break;
                case 'q':
                    pieceList->push_back(Piece(PieceType::Queen, pieceColor, Vec2(col - 1, row - 1), false));
                    break;
                case 'k':
                    pieceList->push_back(Piece(PieceType::King, pieceColor, Vec2(col - 1, row - 1), true));
                    break;
                default:
                    wxLogError("kutzooi dit stuk bestaat niet");
                    break;
                }

                col++;
            }
        }

        // Parse other information
        position.activeColor = tokens[1][0];
        position.castlingRights = tokens[2];
        position.enPassantTarget = tokens[3];
        position.halfmoveClock = std::stoi(tokens[4]);
        position.fullmoveNumber = std::stoi(tokens[5]);

        return position;
    }

    void calcMoves(wxDC &dc)
    {
        // Knight piece;
        // piece.position = Vec2{6, 3};

        // generateMoves(piece, dc);

        // piece.position = Vec2{7, 7};
        // generateMoves(piece, dc);
    }

private:
    void drawRect(wxDC &dc, Vec2 position, wxColor color)
    {
        dc.SetBrush(wxBrush(color));
        dc.DrawRectangle((position.x) * 128, 1024 - (position.y + 1) * 128, 128, 128);
    }

    // int directions[8] = {8, 9, 1, -7, -8, -9, -1, 7};
    int square[64];
    std::vector<Piece> whitePieces;
    std::vector<Piece> blackPieces;

    struct Move
    {
        Move(Vec2 startSquare, Vec2 targetSquare) : startSquare(startSquare), targetSquare(targetSquare) {}

        Vec2 startSquare;
        Vec2 targetSquare;
    };

    // bool isLegal(int dir, int square, int distance) {}

    void generateMoves(Piece &piece, wxDC &dc)
    {
        std::vector<int8_t> distances(4);
        // piece.getDistanceToEdges(distances);
        std::vector<Vec2> &directions = piece.getDirections();

        drawRect(dc, piece.position, wxColour(0, 0, 200));

        if (piece.singleStep)
        {
            for (Vec2 dir : directions)
            {
                Vec2 tempMove = piece.position + dir;
                if (tempMove.x <= 7 && tempMove.x >= 0 && tempMove.y <= 7 && tempMove.y >= 0)
                {
                    Move move(piece.position, piece.position + dir);
                    drawRect(dc, move.targetSquare, wxColour(200, 0, 0));
                }
            }
        }
    }
};

class Pieces
{
public:
    wxBitmap blackBishop;
    wxBitmap blackKing;
    wxBitmap blackKnight;
    wxBitmap blackPawn;
    wxBitmap blackQueen;
    wxBitmap blackRook;
    wxBitmap whiteBishop;
    wxBitmap whiteKing;
    wxBitmap whiteKnight;
    wxBitmap whitePawn;
    wxBitmap whiteQueen;
    wxBitmap whiteRook;

    void loadPieces()
    {
        wxImage::AddHandler(new wxPNGHandler);
        wxImage chessPieceImage;

        bool success = true;

        success &= chessPieceImage.LoadFile("D:/projects/git/chessEngine/pieces-basic-png/black-bishop.png", wxBITMAP_TYPE_PNG);
        blackBishop = wxBitmap(chessPieceImage);

        success &= chessPieceImage.LoadFile("D:/projects/git/chessEngine/pieces-basic-png/black-king.png", wxBITMAP_TYPE_PNG);
        blackKing = wxBitmap(chessPieceImage);

        success &= chessPieceImage.LoadFile("D:/projects/git/chessEngine/pieces-basic-png/black-knight.png", wxBITMAP_TYPE_PNG);
        blackKnight = wxBitmap(chessPieceImage);

        success &= chessPieceImage.LoadFile("D:/projects/git/chessEngine/pieces-basic-png/black-pawn.png", wxBITMAP_TYPE_PNG);
        blackPawn = wxBitmap(chessPieceImage);

        success &= chessPieceImage.LoadFile("D:/projects/git/chessEngine/pieces-basic-png/black-queen.png", wxBITMAP_TYPE_PNG);
        blackQueen = wxBitmap(chessPieceImage);

        success &= chessPieceImage.LoadFile("D:/projects/git/chessEngine/pieces-basic-png/black-rook.png", wxBITMAP_TYPE_PNG);
        blackRook = wxBitmap(chessPieceImage);

        success &= chessPieceImage.LoadFile("D:/projects/git/chessEngine/pieces-basic-png/white-bishop.png", wxBITMAP_TYPE_PNG);
        whiteBishop = wxBitmap(chessPieceImage);

        success &= chessPieceImage.LoadFile("D:/projects/git/chessEngine/pieces-basic-png/white-king.png", wxBITMAP_TYPE_PNG);
        whiteKing = wxBitmap(chessPieceImage);

        success &= chessPieceImage.LoadFile("D:/projects/git/chessEngine/pieces-basic-png/white-knight.png", wxBITMAP_TYPE_PNG);
        whiteKnight = wxBitmap(chessPieceImage);

        success &= chessPieceImage.LoadFile("D:/projects/git/chessEngine/pieces-basic-png/white-pawn.png", wxBITMAP_TYPE_PNG);
        whitePawn = wxBitmap(chessPieceImage);

        success &= chessPieceImage.LoadFile("D:/projects/git/chessEngine/pieces-basic-png/white-queen.png", wxBITMAP_TYPE_PNG);
        whiteQueen = wxBitmap(chessPieceImage);

        success &= chessPieceImage.LoadFile("D:/projects/git/chessEngine/pieces-basic-png/white-rook.png", wxBITMAP_TYPE_PNG);
        whiteRook = wxBitmap(chessPieceImage);
    }

private:
};

// Define a custom panel for drawing
class MyPanel : public wxPanel
{
public:
    MyPanel(wxFrame *parent) : wxPanel(parent)
    {
        pieces.loadPieces();
        // LoadFile(wxInputStream & stream, wxBitmapType type = wxBITMAP_TYPE_ANY, int index = -1)
    }

    void OnPaint(wxPaintEvent &event)
    {
        wxPaintDC dc(this);
        // Draw the chessboard
        DrawChessboard(dc);

        // Draw the PNG image directly on the panel
    }

    void OnMouseDown(wxMouseEvent &event)
    {
        event.GetPosition();
    }

private:
    void DrawChessboard(wxDC &dc)
    {
        Board board;

        wxTextCtrl *control = new wxTextCtrl();
        wxStreamToTextRedirector redirect(control);

        std::map<char, wxBitmap> my_map = {
            {'b', pieces.blackBishop},
            {'r', pieces.blackRook},
            {'n', pieces.blackKnight},
            {'k', pieces.blackKing},
            {'q', pieces.blackQueen},
            {'p', pieces.blackPawn},
            {'B', pieces.whiteBishop},
            {'R', pieces.whiteRook},
            {'N', pieces.whiteKnight},
            {'K', pieces.whiteKing},
            {'Q', pieces.whiteQueen},
            {'P', pieces.whitePawn}};

        // Define your own colors (light gray and dark gray in this example)
        wxColour lightColor(220, 190, 190); // RGB values (0-255)
        wxColour darkColor(150, 100, 100);  // RGB values (0-255)

        // Define the size of each square
        int squareSize = 128;

        // Example FEN string
        std::string fenString = "rnbqkbnr/pppppppp/8/8/4P1N1/8/PPPP1PPP/RNBQKBNR w - - 0 0";
        // std::string fenString = "4r3/2P3R1/R1N2k1P/5Np1/K1p1p3/1pr5/3P4/Bn3Q2 w - - 0 1";

        // Parse the FEN string
        ChessPosition chessPosition = board.parseFEN(fenString);

        // Display parsed information
        std::cout << "Parsed FEN Information:\n";
        std::cout << "Active Color: " << chessPosition.activeColor << "\n";
        std::cout << "Castling Rights: " << chessPosition.castlingRights << "\n";
        std::cout << "En Passant Target: " << chessPosition.enPassantTarget << "\n";
        std::cout << "Halfmove Clock: " << chessPosition.halfmoveClock << "\n";
        std::cout << "Fullmove Number: " << chessPosition.fullmoveNumber << "\n";

        // Display the board
        /*std::cout << "Board:\n";
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                std::cout << chessPosition.board[i][j] << " ";
            }
            std::cout << "\n";
        }*/

        // Draw a chessboard pattern with alternating squares
        for (int row = 0; row < 8; ++row)
        {
            for (int col = 0; col < 8; ++col)
            {
                // Set the custom color for the brush based on the square position
                wxColour squareColor = ((row + col) % 2 == 0) ? lightColor : darkColor;
                dc.SetBrush(wxBrush(squareColor));

                // Calculate the position of each square
                int x = col * squareSize;
                int y = row * squareSize;

                // Draw the rectangle at the calculated position
                dc.DrawRectangle(x, y, squareSize, squareSize);
                if (chessPosition.board[row][col] != '0')
                {
                    dc.DrawBitmap(my_map[chessPosition.board[row][col]], x, y);
                }
            }
        }
        board.calcMoves(dc);
    }

    Pieces pieces;

    // Event table declaration
    wxDECLARE_EVENT_TABLE();

    // wxBitmap chessPieceBitmap;
};

wxBEGIN_EVENT_TABLE(MyPanel, wxPanel)
    EVT_PAINT(MyPanel::OnPaint)
        EVT_LEFT_DOWN(MyPanel::OnMouseDown)

            wxEND_EVENT_TABLE()

    // Define the main frame
    class MyFrame : public wxFrame
{
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, "Chessboard Example", wxDefaultPosition, wxSize(1040, 1070))
    {
        // Create a panel and set it as the main window content
        wxPanel *panel = new MyPanel(this);
    }
};

// Define the application
class MyApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        MyFrame *frame = new MyFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
