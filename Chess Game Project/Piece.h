#ifndef PIECE_H
#define PIECE_H

class Piece
{
public:
    Piece(char cColor);
    virtual ~Piece();
    virtual char GetPiece() = 0;
    virtual bool HasMoved() const;
    char GetColor();
    bool IsLegalMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]);

private:
    virtual bool AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) = 0;
    char color;
};

#endif // PIECE_H