#include "game.hpp"

int cellSize = 100;

Game::Game() {
    InitAudioDevice();
    moveSound = LoadSound("Sounds/move.mp3");
    captureSound = LoadSound("Sounds/capture.mp3");
    checkSound = LoadSound("Sounds/move-check.mp3");

    InitPieces();
    CalculateLegalMoves();
}

Game::~Game() {
    UnloadSound(moveSound);
    UnloadSound(captureSound);
    CloseAudioDevice();
}

void Game::HandleInput() {
    if (gameStatus == GameStatus::playing && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) handleMouseClick(GetMouseX() / cellSize, GetMouseY() / cellSize);
}

void Game::Update() {
    if(hasBoardChanged) {
        CalculateLegalMoves();
        hasBoardChanged = false;
        chessboard.ShowSquares();
    }
}

void Game::Draw() {
    BeginDrawing();
    chessboard.Draw();

    if (clickedPiece) {
        int x = clickedPiece->position.x;
        int y = clickedPiece->position.y;
        DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, SetClickedColor(x, y));
        DrawLegalMoves();
    };

    for (auto p : pieces) {
        p->Draw();
    }

    if(gameStatus != GameStatus::playing){
        GameOver();
    } 

    EndDrawing();
}

void Game::InitPieces() {
    addPiece(Rook::CreateBlack(0,0));
    addPiece(Horse::CreateBlack(1,0));
    addPiece(Bishop::CreateBlack(2,0));
    addPiece(Queen::CreateBlack(4,0));
    addPiece(Bishop::CreateBlack(5,0));
    addPiece(Horse::CreateBlack(6,0));
    addPiece(Rook::CreateBlack(7,0));

    addPiece(Rook::CreateWhite(0,7));
    addPiece(Horse::CreateWhite(1,7));
    addPiece(Bishop::CreateWhite(2,7));
    addPiece(Queen::CreateWhite(4,7));
    addPiece(Bishop::CreateWhite(5,7));
    addPiece(Horse::CreateWhite(6,7));
    addPiece(Rook::CreateWhite(7,7));

    for (int i = 0; i < 8; ++i) {
        addPiece(Pawn::CreateBlack(i, 1));
        addPiece(Pawn::CreateWhite(i, 6));
    }
    //we adding to the end so the king is the last piece to be drawn and when we calculate the legal moves we can use the atackedBy arrays
    addPiece(King::CreateWhite(3,7));
    addPiece(King::CreateBlack(3,0));
}

void Game::addPiece(std::shared_ptr<Piece> piece) {
    pieces.push_back(piece);
    chessboard.grid[(int)piece->position.x][(int)piece->position.y] = piece;
}

void Game::GameOver() {
    DrawRectangle(0, 0, 800, 800, Fade(BLACK, 0.8f));
    DrawText("GAME OVER", 240, 300, 50, WHITE);
    if (gameStatus == GameStatus::whiteWin){
        DrawText("WHITE WINS", 238, 380, 50, WHITE);
    } else if (gameStatus == GameStatus::blackWin){
        DrawText("BLACK WINS", 238, 380, 50, WHITE);
    } else if (gameStatus == GameStatus::STALEMATE){
        DrawText("STALEMATE", 237, 380, 50, WHITE);
    }
}

void Game::handleMouseClick(int x, int y) {
    bool isPieceClick = chessboard.grid[x][y].get();
    bool isOwnPieceClick = isPieceClick && ( chessboard.grid[x][y]->color == ColorTurn);

    if (isOwnPieceClick) {
        clickedPiece = chessboard.grid[x][y];
        return;
    }

    if (clickedPiece && IsLegalMove(x, y)) {
        if (isPieceClick) CapturePiece(x, y);
        MakeMove(x, y);
    }
}

bool Game::IsLegalMove(float x, float y) {
    return std::any_of(clickedPiece->legalMoves.begin(), clickedPiece->legalMoves.end(), [&](auto move) { return Vector2Equals(move, {x, y}); });
}

void Game::CapturePiece(int x, int y) {
    PlaySound(captureSound);
    pieces.erase(std::find(pieces.begin(), pieces.end(), chessboard.grid[x][y]));
}

void Game::MakeMove(int x, int y) {
    chessboard.lastMovePositions[0] = {clickedPiece->position.x, clickedPiece->position.y};
    chessboard.lastMovePositions[1] = {(float)x, (float)y};
    chessboard.grid[x][y] = clickedPiece;
    chessboard.grid[(int)clickedPiece->position.x][(int)clickedPiece->position.y] = nullptr;
    clickedPiece->position = {(float)x, (float)y};
    clickedPiece = nullptr;

    ColorTurn = (ColorTurn == PieceColor::white) ?  PieceColor::black : PieceColor::white;
    PlaySound(moveSound);

    hasBoardChanged = true;
}

void Game::CalculateLegalMoves() {
    std::cout << "i start CalculateLegalMoves" << std::endl;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            chessboard.atackedByBlack[i][j]=false;
            chessboard.atackedByWhite[i][j]=false;
        }
    }
    int i =0;
    while(i < pieces.size()){
        if(pieces[i]->color == PieceColor::white){
            pieces[i]->SetLegalMoves(chessboard.grid, chessboard.atackedByWhite);
        } else{
            pieces[i]->SetLegalMoves(chessboard.grid, chessboard.atackedByBlack);
        }
        i++;
    }
    King::SetKingLegalMoves(pieces[i-2]->legalMoves,chessboard.atackedByBlack);
    if(chessboard.atackedByBlack[(int)pieces[i-2]->position.x][(int)pieces[i-2]->position.y]){
        PlaySound(checkSound);
        std::cout << "black king is in check" << std::endl;
        if(pieces[i-2]->legalMoves.size() == 0){
            gameStatus = GameStatus::blackWin;
        }
    }
    King::SetKingLegalMoves(pieces[i-1]->legalMoves,chessboard.atackedByWhite);
    if(chessboard.atackedByWhite[(int)pieces[i-1]->position.x][(int)pieces[i-1]->position.y]){
        PlaySound(checkSound);
        std::cout << "white king is in check" << std::endl;
        if(pieces[i-1]->legalMoves.size() == 0){
            gameStatus = GameStatus::whiteWin;
        }
    }
    std::cout << "i end CalculateLegalMoves" << std::endl;
}

void Game::DrawLegalMoves() {
    for (auto move : clickedPiece->legalMoves) {
        float x = move.x * cellSize + cellSize / 2;
        float y = move.y * cellSize + cellSize / 2;
        if (chessboard.grid[(int)move.x][(int)move.y]){
            DrawRing({x, y}, 40, 50, 0, 360, 32, Fade(BLACK, 0.1f));
        } else{
            DrawCircle(x, y, 17, Fade(BLACK, 0.1f));
        }
    }
}