#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece
{
public:
    King(char color);
    ~King();

    bool HasMoved() const override;
    void SetMoved(bool hasMoved);

private:
    virtual char GetPiece() override;
    bool AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override;

    bool hasMoved;
};

#endif // KING_H