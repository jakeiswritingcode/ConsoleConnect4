// connect4_model.h
// by Jake Charles Osborne III
#pragma once



#include <bitset>
#include <array>



namespace connect4::model {

	struct Board {
		// each board is stored as a 6x7 bitset with an 8th column on the right edge
		// to prevent wraping when bitshifting the values to detect matches and
		// shortened by one bit as the last bit in the last column is never used
		std::bitset<47> player1;
		std::bitset<47> player2;
		short turnCount = 0;

		short getActivePlayer();
		bool colAvailable(short col);

		std::array<std::array<short, 7>, 6> getBoardGrid() const;
		void addPiece(short);

		short checkForWin() const;

		void reset();
	};

}