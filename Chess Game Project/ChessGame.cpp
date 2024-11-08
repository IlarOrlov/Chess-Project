#include "ChessGame.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

#include <iostream>

ChessGame::ChessGame() : playerTurn('W'), gameBoard() {}

ChessGame::~ChessGame() {}

void ChessGame::ClearConsole()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ChessGame::Start(PipeCommunicator& communicator)
{
    do {
        GetNextMove(gameBoard.board, communicator);
        AlternateTurn();
    } while (!IsGameOver(communicator));
    gameBoard.Print();
}

Board& ChessGame::GetGameBoard()
{
	return gameBoard;
}

char ChessGame::GetPlayerTurn()
{
	return playerTurn;
}

void ChessGame::GetNextMove(Piece* board[8][8], PipeCommunicator& communicator)
{
	bool validMove = false;
	int protocolResult = 0;
	gameBoard.Print();

	// Get input and convert to coordinates
	std::cout << (playerTurn == 'W' ? "White" : "Black") << "'s Move: ";
	std::string readResult = communicator.readFromPipe();
	std::string strStartMove;
	//std::cin >> strStartMove;
	strStartMove = readResult.substr(0, 2);

	// Convert the first character to lowercase
	strStartMove[0] = tolower(strStartMove[0]);
	int startRow = (strStartMove[1] - '0') - 1;
	int startCol = strStartMove[0] - 'a';

	std::cout << "To: ";
	std::string strEndMove;
	//std::cin >> strEndMove;
	strEndMove = readResult.substr(2, 2);
	// Convert the first character to lowercase
	strEndMove[0] = tolower(strEndMove[0]);
	int endRow = (strEndMove[1] - '0') - 1;
	int endCol = strEndMove[0] - 'a';

	// Check that the indices are in range
	// and that the source and destination are different
	if ((startRow >= 0 && startRow <= 7) &&
		(startCol >= 0 && startCol <= 7) &&
		(endRow >= 0 && endRow <= 7) &&
		(endCol >= 0 && endCol <= 7)) {
		// Additional checks in here
		Piece* currPiece = board[startRow][startCol];
		// Check that the piece is the correct color
		if ((currPiece != 0) && (currPiece->GetColor() == playerTurn)) {
			// Check that the destination is a valid destination
			if (currPiece->IsLegalMove(startRow, startCol, endRow, endCol, board)) {
				// Check if the move is a castling move
				if (dynamic_cast<King*>(currPiece) != nullptr && currPiece->HasMoved() == false) {
					// King-side castling
					if (endCol == 6 && startCol == 4) {
						board[startRow][5] = board[startRow][7];
						board[startRow][7] = nullptr;
					}
					// Queen-side castling
					else if (endCol == 2 && startCol == 4) {
						board[startRow][3] = board[startRow][0];
						board[startRow][0] = nullptr;
					}
				}

				// Make the move
				Piece* temp = board[endRow][endCol];
				board[endRow][endCol] = board[startRow][startCol];
				board[startRow][startCol] = nullptr;

				// Check for pawn promotion
				if (dynamic_cast<Pawn*>(board[endRow][endCol]) != nullptr &&
					(endRow == 0 || endRow == 7))
				{
					protocolResult = 11;
					// Prompt the user for the promoted piece
					std::cout << "Choose a piece for promotion (Q, R, B, N): ";

					delete board[endRow][endCol];
					board[endRow][endCol] = new Pawn(playerTurn);
					gameBoard.Print();
				}

				// Make sure that the current player is not in check
				if (!gameBoard.IsInCheck(playerTurn)) {
					delete temp;
					validMove = true;
				}
				else { // Undo the last move
					protocolResult = 4;
					board[startRow][startCol] = board[endRow][endCol];
					board[endRow][endCol] = temp;

					// Undo castling moves
					if (dynamic_cast<King*>(board[startRow][startCol]) != nullptr) {
						// King-side castling
						if (endCol == 6 && startCol == 4) {
							board[startRow][7] = board[startRow][5];
							board[startRow][5] = nullptr;
						}
						// Queen-side castling
						else if (endCol == 2 && startCol == 4) {
							board[startRow][0] = board[startRow][3];
							board[startRow][3] = nullptr;
						}
					}
				}
			}
			if (!validMove) protocolResult = 6;
			if (strStartMove == strEndMove) {
				protocolResult = 7;
			}
		}
		else protocolResult = 2;
	}
	else {
		protocolResult = 5;
	}
	ClearConsole(); // Clear console after each move
	if (gameBoard.IsInCheck(playerTurn == 'W' ? 'B' : 'W')) {
		// Check message
		protocolResult = 1;
		std::cout << "Check! " << (playerTurn == 'W' ? "Black" : "White") << " is in check." << std::endl;
	}
	else if (gameBoard.IsInCheck(playerTurn)) {
		// Check message
		protocolResult = 10;
		std::cout << "Check! " << (playerTurn == 'W' ? "White" : "Black") << " is in check." << std::endl;
	}
	else if (!validMove) {
		// Why illegal move message
		std::cout << "Illegal move! Please make a valid move." << std::endl;
	}
	else if (protocolResult != 11) protocolResult = 0;
	communicator.writeToPipe(std::to_string(protocolResult));
	IsGameOver(communicator);
}

void ChessGame::AlternateTurn()
{
	playerTurn = (playerTurn == 'W') ? 'B' : 'W';
}

bool ChessGame::IsGameOver(PipeCommunicator& communicator)
{
	// Check that the current player can move
	// If not, we have a stalemate or checkmate
	bool canMove(false);
	AlternateTurn();
	canMove = gameBoard.CanMove(playerTurn);
	if (!canMove) {
		if (gameBoard.IsInCheck(playerTurn)) {
			AlternateTurn();
			std::cout << "Checkmate, " << (playerTurn == 'W' ? "White" : "Black") << " Wins!" << std::endl;
			communicator.writeToPipe("8");
		}
		else {
			// Stalemate message
			std::cout << "Stalemate! The game ends in a draw." << std::endl;
			communicator.writeToPipe("9");
		}
	}
	AlternateTurn();
	return !canMove;
}

void ChessGame::PromotePawn(Piece* board[8][8], PipeCommunicator& communicator) {
	// Find the pawn on the specified tile
	std::string readResult = communicator.readFromPipe();
	std::string pawnPosition = readResult.substr(0, 2);
	std::string promotionPosition = readResult.substr(2, 2);
	pawnPosition[0] = tolower(pawnPosition[0]);
	promotionPosition[0] = tolower(promotionPosition[0]);

	int pawnRow = (pawnPosition[1] - '0') - 1;
	int pawnColumn = pawnPosition[0] - 'a';

	Piece* pawn = board[pawnRow][pawnColumn];
	std::string exitCode = "0";

	// Check if the piece on the specified tile is a pawn before promoting
	if (pawn && pawn->GetPiece() == 'P') {
		// Your logic for promoting the pawn to the desired piece (default to queen if not specified)
		Piece* promotionPiece = board[(promotionPosition[1] - '0') - 1][promotionPosition[0] - 'a'];
		char promotionPieceType = 'T';
		if (promotionPiece != nullptr) {
			promotionPieceType = promotionPiece->GetPiece();
		}

		switch (promotionPieceType)
		{
		case 'Q':
			delete board[pawnRow][pawnColumn];
			board[pawnRow][pawnColumn] = new Queen(playerTurn);
			break;
		case 'R':
			delete board[pawnRow][pawnColumn];
			board[pawnRow][pawnColumn] = new Rook(playerTurn);
			break;
		case 'B':
			delete board[pawnRow][pawnColumn];
			board[pawnRow][pawnColumn] = new Bishop(playerTurn);
			break;
		case 'N':
			delete board[pawnRow][pawnColumn];
			board[pawnRow][pawnColumn] = new Knight(playerTurn);
			break;
		default:
			// Default to queen for invalid input
			delete board[pawnRow][pawnColumn];
			board[pawnRow][pawnColumn] = new Queen(playerTurn);
			exitCode = "1";
			break;
		}

		communicator.writeToPipe(exitCode);
	}
}
