#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

class Queen : public Piece
{
public:
    Queen(char color);
    ~Queen();

private:
    virtual char GetPiece() override;
    bool AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override;
};

#endif // QUEEN_H