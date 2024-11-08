#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"

class Board
{
public:
    Board();
    ~Board();

    void Print();
    bool IsInCheck(char color);
    bool CanMove(char color);

    Piece* board[8][8];
};

#endif // BOARD_H