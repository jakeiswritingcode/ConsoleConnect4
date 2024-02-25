// connect4_model_state.cpp
// by Jake Charles Osborne III



#include "connect4_model.h"
#include <memory>
#include <vector>
#include <stdexcept>

using std::shared_ptr;
using std::vector;



namespace connect4::model {

	shared_ptr<ai::State> Board::clone() const {
		return std::make_shared<Board>(*this);
	};

	vector<shared_ptr<ai::Action>> Board::getActions() const {
		vector<shared_ptr<ai::Action>> actions;

		for (int i = 0; i < 7; ++i) {
			if (!colAvailable(i)) continue;

			shared_ptr<ai::Action> action = std::make_shared<Move>(i);
			actions.push_back(action);
		}

		return actions;
	}

	double Board::useAction(shared_ptr<ai::Action> actionPtr) {
		Move* movePtr = dynamic_cast<Move*>(actionPtr.get());
		if (!movePtr) throw std::logic_error("invalid ai::Action passed as an argument");
		Move move = *movePtr;

		short player = getActivePlayer();

		addPiece(move.col);

		short winner = checkForWin();
		if (winner == 0 || winner == -1) {
			return -1;
		}
		else if (winner == player) {
			return 100;
		}
		else {
			return -100;
		}
	}

	bool Board::isTerminal() const {
		return checkForWin() != 0;
	}

}