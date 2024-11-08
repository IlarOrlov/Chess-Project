#include "Pawn.h"
#include <cmath>

Pawn::Pawn(char color) : Piece(color), hasMoved(false) {}

Pawn::~Pawn() {}

char Pawn::GetPiece() {
    return 'P';
}

bool Pawn::AreSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
    Piece* qpDest = board[destRow][destCol];

    if (qpDest == nullptr) {
        // Destination square is unoccupied
        if (srcCol == destCol) {
            int rowDifference = GetColor() == 'W' ? destRow - srcRow : srcRow - destRow;

            // Check if the pawn is moving one square forward or two squares forward at the beginning
            if ((rowDifference == 1 && GetColor() == 'W') || (rowDifference == 2 && srcRow == 1 && GetColor() == 'W') ||
                (rowDifference == 1 && GetColor() == 'B') || (rowDifference == 2 && srcRow == 6 && GetColor() == 'B')) {
                hasMoved = true; // Mark the pawn as moved
                return true;
            }
        }
        else if (std::abs(destCol - srcCol) == 1 && std::abs(destRow - srcRow) == 1) {
            // En passant capture
            int direction = GetColor() == 'W' ? 1 : -1;
            if ((srcRow == 4 && GetColor() == 'W') || (srcRow == 3 && GetColor() == 'B')) {
                // Check if the move is a double-step and there is an opponent's pawn on the adjacent file
                Piece* oppPawn = board[srcRow][destCol];
                if (oppPawn != nullptr && oppPawn->GetColor() != GetColor() && dynamic_cast<Pawn*>(oppPawn) != nullptr && dynamic_cast<Pawn*>(oppPawn)->HasMovedTwoSquares()) {
                    // Capture the pawn on the adjacent square
                    board[srcRow][destCol] = nullptr;
                    return true;
                }
            }
        }
    }
    else {
        // Destination square is occupied
        if ((std::abs(destCol - srcCol) == 1) && (std::abs(destRow - srcRow) == 1)) {
            // Diagonal capture
            if (GetColor() == 'W' && destRow == srcRow + 1 && qpDest->GetColor() == 'B') {
                return true;
            }
            else if (GetColor() == 'B' && destRow == srcRow - 1 && qpDest->GetColor() == 'W') {
                return true;
            }
        }
    }

    return false;
}

bool Pawn::HasMovedTwoSquares() const {
    return hasMoved;
}