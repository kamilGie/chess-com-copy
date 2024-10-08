#pragma once
#include <raylib.h>
#include <memory>

class Piece;

class MomentoMove {
   public:
    Vector2 from;
    Vector2 to;
    std::shared_ptr<Piece> capturedPiece=nullptr;
    bool promotion = false;
    MomentoMove(Vector2 from, Vector2 to, std::shared_ptr<Piece> capturedPiece=nullptr);
};