#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "Board.h"
#include "PipeCommunicator.h"

class ChessGame
{
public:
    ChessGame();
    ~ChessGame();

    void ClearConsole();
    void Start(PipeCommunicator& communicator);
    void GetNextMove(Piece* board[8][8], PipeCommunicator& communicator);
    void AlternateTurn();
    bool IsGameOver(PipeCommunicator& communicator);
    void PromotePawn(Piece* board[8][8], PipeCommunicator& communicator);

    Board& GetGameBoard();
    char GetPlayerTurn();

private:
    Board gameBoard;
    char playerTurn;
};

#endif // CHESSGAME_H