#include "King.h"

King::King(char color) : Piece(color), hasMoved(false) {}

King::~King() {}

char King::GetPiece() {
    return 'K';
}

bool King::HasMoved() const {
    return hasMoved;
}

void King::SetMoved(bool hasMoved) {
    hasMoved = hasMoved;
}

bool King::AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
    // Regular king moves
    int rowDelta = destRow - srcRow;
    int colDelta = destCol - srcCol;
    if (((rowDelta >= -1) && (rowDelta <= 1)) &&
        ((colDelta >= -1) && (colDelta <= 1))) {
        return true;
    }

    // Castling
    if (!hasMoved && srcRow == destRow) {
        // King-side castling
        if (destCol == 6 && srcCol == 4 && board[srcRow][7] != nullptr && !board[srcRow][7]->HasMoved()) {
            for (int i = srcCol + 1; i < destCol; ++i) {
                if (board[srcRow][i] != nullptr) {
                    return false;
                }
            }
            return true;
        }
        // Queen-side castling
        else if (destCol == 2 && srcCol == 4 && board[srcRow][0] != nullptr && !board[srcRow][0]->HasMoved()) {
            for (int i = srcCol - 1; i > destCol; --i) {
                if (board[srcRow][i] != nullptr) {
                    return false;
                }
            }
            return true;
        }
    }

    return false;
}