// connect4_model.cpp
// by Jake Charles Osborne III



#include "connect4_model.h"
#include <bitset>
#include <array>

using std::bitset;
using std::array;



namespace connect4::model {

	short Board::getActivePlayer() const {
		return turnCount % 2 + 1;
	}

	bool Board::colAvailable(short col) const {
		if (col < 0 || col > 6) return false;
		return player1[40 + col] == 0 && player2[40 + col] == 0;
	}

	array<array<short, 7>, 6> Board::getBoardGrid() const {
		array<array<short, 7>, 6> board;
		for (short row = 0; row < 6; ++row) {
			for (short col = 0; col < 7; ++col) {
				if (player1[row * 8 + col]) {
					board[row][col] = 1;
				}
				else if (player2[row * 8 + col]) {
					board[row][col] = 2;
				}
				else {
					board[row][col] = 0;
				}
			}
		}
		return board;
	}

	void Board::addPiece(short col) { // 0 order parameter
		for (int i = 0; i < 7; ++i) {
			if (!player1[i * 8 + col] && !player2[i * 8 + col]) {
				if (turnCount % 2 == 0) { // player 1's turn
					player1.set(i * 8 + col);
					++turnCount;
					break;
				}
				else { // player 2's turn
					player2.set(i * 8 + col);
					++turnCount;
					break;
				}
			}
		}
	}

	short Board::checkForWin() const {
		if ((player1 & (player1 << 1) & (player1 << 2) & (player1 << 3)).any() || // horizontal match
			(player1 & (player1 << 8) & (player1 << 16) & (player1 << 24)).any() || // vertical match
			(player1 & (player1 << 9) & (player1 << 18) & (player1 << 27)).any() || // diagonalmatch
			(player1 & (player1 << 7) & (player1 << 14) & (player1 << 21)).any()) // antidiagonal match
		{
			return 1;
		}

		if ((player2 & (player2 << 1) & (player2 << 2) & (player2 << 3)).any() || // horizontal match
			(player2 & (player2 << 8) & (player2 << 16) & (player2 << 24)).any() || // vertical match
			(player2 & (player2 << 9) & (player2 << 18) & (player2 << 27)).any() || // diagonal match
			(player2 & (player2 << 7) & (player2 << 14) & (player2 << 21)).any()) // antidiagonalmatch
		{
			return 2;
		}

		for (short i = 0; i < 7; ++i) {
			if (player1[40 + i] == 0 && player2[40 + i] == 0) return 0; // no win yet
		}

		return -1; // tie
	}

	void Board::reset() {
		turnCount = 0;
		player1.reset();
		player2.reset();
	}

}

