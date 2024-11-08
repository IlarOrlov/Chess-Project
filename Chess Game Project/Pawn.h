#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn : public Piece
{
public:
    Pawn(char color);
    ~Pawn();

private:
    bool hasMoved; // Added to track whether the pawn has moved or not

    virtual char GetPiece() override;
    bool AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override;

    bool HasMovedTwoSquares() const;
};

#endif // PAWN_H