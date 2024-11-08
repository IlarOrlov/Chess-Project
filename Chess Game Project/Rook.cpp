#include "Rook.h"

Rook::Rook(char color) : Piece(color) {}

Rook::~Rook() {}

char Rook::GetPiece() {
    return 'R';
}

bool Rook::AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
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
    return false;
}