#include "Knight.h"

Knight::Knight(char color) : Piece(color) {}

Knight::~Knight() {}

char Knight::GetPiece() {
    return 'N';
}

bool Knight::AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
    // Destination square is unoccupied or occupied by opposite color
    if ((srcCol == destCol + 1) || (srcCol == destCol - 1)) {
        if ((srcRow == destRow + 2) || (srcRow == destRow - 2)) {
            return true;
        }
    }
    if ((srcCol == destCol + 2) || (srcCol == destCol - 2)) {
        if ((srcRow == destRow + 1) || (srcRow == destRow - 1)) {
            return true;
        }
    }
    return false;
}