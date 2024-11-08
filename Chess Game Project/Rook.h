#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece
{
public:
    Rook(char color);
    ~Rook();

private:
    virtual char GetPiece() override;
    bool AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override;
};

#endif // ROOK_H