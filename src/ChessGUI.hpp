/*// ChessGUI.hpp

#ifndef CHESS_GUI_HPP
#define CHESS_GUI_HPP

#include <wx/wx.h>
#include <vector>
#include "ChessBoard.hpp"

struct ChessPosition
{
    char board[8][8];
    char activeColor; // 'w' for white, 'b' for black
    std::string castlingRights;
    std::string enPassantTarget;
    int halfmoveClock;
    int fullmoveNumber;
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

    void loadPieces();
};

class Square
{
public:
    Square(Vec2 position, wxColour color) : position(position), color(color) {}
    Vec2 position;
    wxColour color;
};

class MyPanel : public wxPanel
{
public:
    std::vector<Square> squares;
    bool redraw = false;

    MyPanel(wxFrame *parent);

    void OnPaint(wxPaintEvent &event);
    void OnMouseDown(wxMouseEvent &event);

private:
    ChessGame game;
    bool makingMove = false;
    Vec2 selectedSquare;

    void DrawChessboard(wxDC &dc);
    Pieces pieces;

    // Event table declaration
    wxDECLARE_EVENT_TABLE();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

#endif // CHESS_GUI_HPP*/
