#include "piece_interfaces.hpp"

#define ORTHOGONAL_MOVES \
    {1, 0}, {0, 1}, {-1, 0}, { 0, -1 }
#define DIAGONALLY_MOVES \
    {1, 1}, {1, -1}, {-1, 1}, { -1, -1 }
#define L_SHAPED_MOVES \
    {1, 2}, {2, 1}, {-2, 1}, {1, -2}, {-1, 2}, {2, -1}, {-1, -2}, { -2, -1 }

// ### KING ### //

class King : public LimitedRangePiece {
   public:
    King(float column, float row, const std::string& pieceName, PieceColor color) : LimitedRangePiece(column, row, pieceName, {ORTHOGONAL_MOVES, DIAGONALLY_MOVES}, color){};
    virtual ~King() = default;
    int getValue() override { return 20; }

    static std::shared_ptr<King> CreateBlack(float column, float row) {
        return std::make_shared<King>(column, row, "kingBlack", PieceColor::black);
    }

    static std::shared_ptr<King> CreateWhite(float column, float row) {
        return std::make_shared<King>(column, row, "kingWhite", PieceColor::white);
    }
};

// ### QUEEN ### //

class Queen : public LongRangePiece {
   public:
    Queen(float column, float row, const std::string& pieceName, PieceColor color) : LongRangePiece(column, row, pieceName, {{ORTHOGONAL_MOVES, DIAGONALLY_MOVES}}, color){};
    virtual ~Queen() = default;
    int getValue() override { return 10; }

    static std::shared_ptr<Queen> CreateBlack(float column, float row) {
        return std::make_shared<Queen>(column, row, "QueenBlack", PieceColor::black);
    }

    static std::shared_ptr<Queen> CreateWhite(float column, float row) {
        return std::make_shared<Queen>(column, row, "QueenWhite", PieceColor::white);
    }
};

// ### ROOK ### //

class Rook : public LongRangePiece {
   public:
    Rook(float column, float row, const std::string& pieceName, PieceColor color) : LongRangePiece(column, row, pieceName, {ORTHOGONAL_MOVES}, color){};
    virtual ~Rook() = default;
    int getValue() override { return 5; }

    static std::shared_ptr<Rook> CreateBlack(float column, float row) {
        return std::make_shared<Rook>(column, row, "RookBlack", PieceColor::black);
    }

    static std::shared_ptr<Rook> CreateWhite(float column, float row) {
        return std::make_shared<Rook>(column, row, "RookWhite", PieceColor::white);
    }
};

// ### HORSE ###//

class Horse : public LimitedRangePiece {
   public:
    Horse(float column, float row, const std::string& pieceName, PieceColor color) : LimitedRangePiece(column, row, pieceName, {L_SHAPED_MOVES}, color){};
    virtual ~Horse() = default;
    int getValue() override { return 3; }

    static std::shared_ptr<Horse> CreateBlack(float column, float row) {
        return std::make_shared<Horse>(column, row, "HorseBlack", PieceColor::black);
    }

    static std::shared_ptr<Horse> CreateWhite(float column, float row) {
        return std::make_shared<Horse>(column, row, "HorseWhite", PieceColor::white);
    }
};

// ### BISHOP ### //

class Bishop : public LongRangePiece {
   public:
    Bishop(float column, float row, const std::string& pieceName, PieceColor color) : LongRangePiece(column, row, pieceName, {DIAGONALLY_MOVES}, color){};
    virtual ~Bishop() = default;
    int getValue() override { return 3; }

    static std::shared_ptr<Bishop> CreateBlack(float column, float row) {
        return std::make_shared<Bishop>(column, row, "BishopBlack", PieceColor::black);
    }

    static std::shared_ptr<Bishop> CreateWhite(float column, float row) {
        return std::make_shared<Bishop>(column, row, "BishopWhite", PieceColor::white);
    }
};

// ### PAWN ### //

class Pawn : public Piece {
   public:
    Pawn(float column, float row, const std::string& pieceName, PieceColor color) : Piece(column, row, pieceName, color){};
    virtual ~Pawn() = default;
    int getValue() override { return 1; }

    Vector2 en_passant = {0,0};

    void SetLegalMoves(std::shared_ptr<Piece> grid[][8]) override {
        legalMoves.clear();

        if (position.y == 7 || position.y == 0) return;
        int moveDirection = color == PieceColor::white ? -1 : 1;
        int x = position.x;
        int y = position.y + moveDirection;
        if (!grid[x][y]) {
            if (SafeMove(x,y,grid))addLegalMove(x, y);
            if (moveCount==0 && !grid[x][y + moveDirection] && SafeMove(x,y+moveDirection,grid)) addLegalMove(x, y + moveDirection);
        }
        if (x > 0 && grid[x - 1][y] && grid[x - 1][y]->color != color && SafeMove(x-1,y,grid)) addLegalMove(x - 1, y);
        if (x < 7  && grid[x + 1][y] && grid[x + 1][y]->color != color && SafeMove(x+1,y,grid)) addLegalMove(x + 1, y);
        if (en_passant.y > 0 && SafeMove(en_passant.x,en_passant.y,grid)) addLegalMove(en_passant.x, en_passant.y);
        en_passant = {0,0};  
    }

    bool SetAtackedPools(std::shared_ptr<Piece> grid[][8], bool atackedPools[8][8]) override {
        int moveDirection = color == PieceColor::white ? -1 : 1;
        int x = position.x;
        int y = position.y + moveDirection;
        if (x > 0 && isInsideBoard(x - 1, y)){
            atackedPools[x - 1][y] = true;
            if (grid[x-1][y] &&  grid[x - 1][y]->color != color && grid[x - 1][y]->getValue() == 20) return true;
        }
        if (x < 7 && isInsideBoard(x + 1, y)){
            atackedPools[x + 1][y] = true;
            if (grid[x+1][y] &&  grid[x + 1][y]->color != color && grid[x + 1][y]->getValue() == 20) return true;
        }
        return false;
    }

    static std::shared_ptr<Pawn> CreateBlack(float column, float row) {
        return std::make_shared<Pawn>(column, row, "pawnBlack", PieceColor::black);
    }

    static std::shared_ptr<Pawn> CreateWhite(float column, float row) {
        return std::make_shared<Pawn>(column, row, "pawnWhite", PieceColor::white);
    }
};