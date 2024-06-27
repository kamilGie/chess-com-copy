#pragma once
#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <vector>

#include "chessboard.hpp"
#include "piece_interfaces.hpp"
#include "pieces.cpp"
#include "memory"

extern int cellSize;

class Game {
   public:
    Game();
    ~Game();
    void Run();

   private:
    void processEvent();
    void handleMouseClick(int x, int y);
    void MakeMove(int x, int y);
    void CapturePiece(int x, int y);
    bool IsLegalMove(float x, float y);
    void Draw();
    void DrawLegalMoves();
    void CalculateLegalMoves(); 
    void InitPieces();
    void addPiece(std::shared_ptr<Piece> piece);

   private:
    Chessboard chessboard = Chessboard();
    std::vector<std::shared_ptr<Piece>> pieces;
    std::shared_ptr<Piece> clickedPiece = nullptr;
    Sound moveSound;
    Sound captureSound;
    bool isWhiteTurn = true;
};