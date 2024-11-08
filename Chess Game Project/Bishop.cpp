#include "Bishop.h"

Bishop::Bishop(char color) : Piece(color) {}

Bishop::~Bishop() {}

char Bishop::GetPiece() {
    return 'B';
}

bool Bishop::AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
    if ((destCol - srcCol == destRow - srcRow) || (destCol - srcCol == srcRow - destRow)) {
        // Make sure that all intervening squares are empty
        int rowOffset = (destRow - srcRow > 0) ? 1 : -1;
        int colOffset = (destCol - srcCol > 0) ? 1 : -1;
        int checkRow;
        int checkCol;
        for (checkRow = srcRow + rowOffset, checkCol = srcCol + colOffset;
            checkRow != destRow;
            checkRow = checkRow + rowOffset, checkCol = checkCol + colOffset)
        {
            if (board[checkRow][checkCol] != nullptr) {
                return false;
            }
        }
        return true;
    }
    return false;
}