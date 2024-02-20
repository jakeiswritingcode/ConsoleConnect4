// connect4_model.h
// by Jake Charles Osborne III
#pragma once



#include "../../AI/Q-Learning/state_action_interface.h"
#include <bitset>
#include <array>



namespace connect4::model {

	struct Move : ai::Action {
		int col;
		Move(int x) : col(x) {}
	};

	class Board : public ai::State{
	public:
		// each board is stored as a 6x7 bitset with an 8th column on the right edge
		// to prevent wraping when bitshifting the values to detect matches and
		// shortened by one bit as the last bit in the last column is never used
		std::bitset<47> player1;
		std::bitset<47> player2;
		short turnCount = 0;

		Board(const Board& board) : player1(board.player1), player2(board.player2), turnCount(board.turnCount) {}
		std::array<std::array<short, 7>, 6> getBoardGrid() const;
		bool colAvailable(short col) const;
		void addPiece(short);
		short getActivePlayer() const;
		short checkForWin() const;
		void reset();

		// State Methods
		std::shared_ptr<ai::State> clone() const override;
		std::vector<std::shared_ptr<ai::Action>> getActions() const override;
		void useAction(std::shared_ptr<ai::Action>) override;
		bool isTerminal() const override;
	};

}