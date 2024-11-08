#include "Piece.h"

Piece::Piece(char cColor) : color(cColor) {}

Piece::~Piece() {}

bool Piece::HasMoved() const
{
    return false; // Default implementation for pieces other than the king
}

char Piece::GetColor()
{
    return color;
}

bool Piece::IsLegalMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8])
{
    Piece* qpDest = board[destRow][destCol];
    if ((qpDest == nullptr) || (color != qpDest->GetColor()))
    {
        return AreSquaresLegal(srcRow, srcCol, destRow, destCol, board);
    }
    return false;
}