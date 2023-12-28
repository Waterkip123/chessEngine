#include <iostream>
#include <sstream>
#include <vector>

#include <map>
#include <algorithm>
#include <random>

#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/cursor.h>
#include <wx/dcbuffer.h>

#include "ChessBoard.hpp"
#include "ChessGame.hpp"
#include "ChessGUI.hpp"
#include "Utils.hpp"
#include "Piece.hpp"

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

class Square
{
public:
    Square(Vec2 position, wxColour color) : position(position), color(color) {}
    Vec2 position;
    wxColour color;
};

// Define a custom panel for drawing
class MyPanel : public wxPanel
{
public:
    // std::vector<Square> squares;
    bool redraw = false;

    MyPanel(wxFrame *parent) : wxPanel(parent)
    {
        pieces.loadPieces();

        // Example FEN string
        std::string fenString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        // std::string fenString = "4r3/2P3R1/R1N2k1P/5Np1/K1p1p3/1pr5/3P4/Bn3Q2 w - - 0 1";

        // Parse the FEN string
        game.parseFEN(fenString);
        // LoadFile(wxInputStream & stream, wxBitmapType type = wxBITMAP_TYPE_ANY, int index = -1)

        game.board.update(game.whitePieces, game.blackPieces);
    }

    void OnPaint(wxPaintEvent &event)
    {
        wxBufferedPaintDC dc(this);
        // Draw the chessboard
        DrawChessboard(dc);
        // for (Square square : squares)
        // {
        //     dc.SetBrush(wxBrush(square.color));
        //     dc.DrawRectangle(square.position.x * 128, square.position.y * 128, 128, 128);
        // }

        // squares.clear();

        // Draw the PNG image directly on the panel
    }

    Vec2 wxPointToVec2(wxPoint point)
    {
        return Vec2((point.x / 128), 7 - (point.y / 128));
    }

    void OnMouseDown(wxMouseEvent &event)
    {
        Vec2 clickedSquare = wxPointToVec2(event.GetPosition());

        if (makingMove)
        {
            if (clickedSquare == movingPiece->position)
            {
                movingPiece = nullptr;
                makingMove = !makingMove;
                squares.clear();
            }

            for (ChessMove move : legalMoves)
            {
                if (clickedSquare == move.targetSquare)
                {
                    game.makeMove(ChessMove(movingPiece->position, clickedSquare));
                    makingMove = !makingMove;
                    squares.clear();
                }
            }
        }
        else
        {
            movingPiece = game.board.board[clickedSquare.x][clickedSquare.y];

            if (movingPiece != nullptr)
            {
                legalMoves.clear();
                game.generateMoves(*movingPiece, legalMoves);

                for (ChessMove move : legalMoves)
                {
                    Square square(move.targetSquare, wxColour(200, 50, 50));
                    squares.push_back(square);
                }

                Square square(clickedSquare, wxColour(150, 200, 100));
                squares.push_back(square);
                makingMove = !makingMove;
            }
        }

        this->Refresh();
    }

    wxColour alphaBlendWxColour(wxColour fg, wxColour bg, float alpha)
    {
        float r = bg.Red() * (1 - alpha) + fg.Red() * alpha;
        float g = bg.Green() * (1 - alpha) + fg.Green() * alpha;
        float b = bg.Blue() * (1 - alpha) + fg.Blue() * alpha;

        wxColour result((uint8_t)r, (uint8_t)g, (uint8_t)b);
        return result;
    }

private:
    ChessGame game;
    bool makingMove = false;
    Vec2 selectedSquare;
    Piece *movingPiece;
    std::vector<ChessMove> legalMoves;
    std::vector<Square> squares;

    void
    DrawChessboard(wxDC &dc)
    {
        wxTextCtrl *control = new wxTextCtrl();
        wxStreamToTextRedirector redirect(control);

        /*std::map<char, wxBitmap> my_map = {
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
            {'P', pieces.whitePawn}};*/

        std::map<PieceType, wxBitmap> white_map = {
            {PieceType::Bishop, pieces.whiteBishop},
            {PieceType::Rook, pieces.whiteRook},
            {PieceType::Knight, pieces.whiteKnight},
            {PieceType::King, pieces.whiteKing},
            {PieceType::Queen, pieces.whiteQueen},
            {PieceType::WhitePawn, pieces.whitePawn}};

        std::map<PieceType, wxBitmap> black_map = {
            {PieceType::Bishop, pieces.blackBishop},
            {PieceType::Rook, pieces.blackRook},
            {PieceType::Knight, pieces.blackKnight},
            {PieceType::King, pieces.blackKing},
            {PieceType::Queen, pieces.blackQueen},
            {PieceType::BlackPawn, pieces.blackPawn}};

        // Define your own colors (light gray and dark gray in this example)
        wxColour lightColor(220, 190, 190); // RGB values (0-255)
        wxColour darkColor(150, 100, 100);  // RGB values (0-255)

        // Define the size of each square
        int squareSize = 128;

        for (int row = 0; row < 8; ++row)
        {
            for (int col = 0; col < 8; ++col)
            {
                // Set the custom color for the brush based on the square position
                wxColour squareColor = ((row + col) % 2 == 0) ? lightColor : darkColor;
                dc.SetBrush(wxBrush(squareColor, wxBRUSHSTYLE_SOLID));

                // Calculate the position of each square
                int x = col * squareSize;
                int y = row * squareSize;

                // Draw the rectangle at the calculated position
                dc.DrawRectangle(x, y, squareSize, squareSize);
            }
        }

        for (Square square : squares)
        {
            float alpha = 0.8;
            wxColour squareColor = ((square.position.x + 7 - square.position.y) % 2 == 0) ? lightColor : darkColor;
            wxColour blendedColour = alphaBlendWxColour(square.color, squareColor, alpha);
            dc.SetBrush(wxBrush(blendedColour));
            dc.DrawRectangle(square.position.x * squareSize, (7 - square.position.y) * squareSize, squareSize, squareSize);
        }

        if (true)
        {
            for (int row = 0; row < 8; ++row)
            {
                for (int col = 0; col < 8; ++col)
                {
                    Piece *piece = game.board.board[col][row];

                    if (piece != nullptr)
                    {
                        if (piece->pieceColor == PieceColor::White)
                        {
                            dc.DrawBitmap(white_map[piece->pieceType], col * 128, 1024 - (row + 1) * 128);
                        }
                        else
                        {
                            dc.DrawBitmap(black_map[piece->pieceType], col * 128, 1024 - (row + 1) * 128);
                        }
                    }
                }
            }
        }
        else
        {

            for (Piece piece : game.whitePieces)
            {
                dc.DrawBitmap(white_map[piece.pieceType], piece.position.x * 128, 1024 - (piece.position.y + 1) * 128);
            }

            for (Piece piece : game.blackPieces)
            {
                dc.DrawBitmap(black_map[piece.pieceType], piece.position.x * 128, 1024 - (piece.position.y + 1) * 128);
            }
            game.calcMoves();
        }
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
        panel->SetDoubleBuffered(true);
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