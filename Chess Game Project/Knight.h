#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece
{
public:
    Knight(char color);
    ~Knight();

private:
    virtual char GetPiece() override;
    bool AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override;
};

#endif // KNIGHT_H