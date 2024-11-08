#include "Queen.h"

Queen::Queen(char color) : Piece(color) {}

Queen::~Queen() {}

char Queen::GetPiece() {
    return 'Q';
}

bool Queen::AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
    if (srcRow == destRow) {
        // Make sure that all intervening squares are empty
        int colOffset = (destCol - srcCol > 0) ? 1 : -1;
        for (int checkCol = srcCol + colOffset; checkCol != destCol; checkCol = checkCol + colOffset) {
            if (board[srcRow][checkCol] != nullptr) {
                return false;
            }
        }
        return true;
    }
    else if (destCol == srcCol) {
        // Make sure that all intervening squares are empty
        int rowOffset = (destRow - srcRow > 0) ? 1 : -1;
        for (int checkRow = srcRow + rowOffset; checkRow != destRow; checkRow = checkRow + rowOffset) {
            if (board[checkRow][srcCol] != nullptr) {
                return false;
            }
        }
        return true;
    }
    else if ((destCol - srcCol == destRow - srcRow) || (destCol - srcCol == srcRow - destRow)) {
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