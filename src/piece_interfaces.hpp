#pragma once
#include <raylib.h>

#include <string>
#include <vector>

extern int cellSize;

class Piece {
   public:
    Piece(float column, float row, const std::string& pieceName);
    virtual ~Piece();

    void Draw();
    virtual int getValue() = 0;
    virtual bool whiteColor() = 0;
    virtual void SetLegalMoves(Piece* grid[][8]) = 0;

    Vector2 position;
    std::vector<Vector2> legalMoves;

   protected:
    void addLegalMove(int x, int y);
    bool isInsideBoard(int x, int y);

   private:
    Texture2D texture;
};

class LongRangePiece : public Piece {
   public:
    LongRangePiece(float column, float row, const std::string& pieceName, std::vector<Vector2> moveDirections) : Piece(column, row, pieceName), moveDirections(moveDirections){};
    virtual ~LongRangePiece() = default;
    void SetLegalMoves(Piece* grid[][8]) override;

   protected:
    std::vector<Vector2> moveDirections;
};

class LimitedRangePiece : public Piece {
   public:
    LimitedRangePiece(float column, float row, const std::string& pieceName, std::vector<Vector2> moveDirections) : Piece(column, row, pieceName), moveDirections(moveDirections){};
    virtual ~LimitedRangePiece() = default;
    void SetLegalMoves(Piece* grid[][8]) override;

   protected:
    std::vector<Vector2> moveDirections;
};