


#include "connect4_model.h"
#include <stdexcept>

using std::vector;
using std::shared_ptr;



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

	void Board::useAction(shared_ptr<ai::Action> actionPtr) {
		Move* movePtr = dynamic_cast<Move*>(actionPtr.get());
		if (!movePtr) throw std::logic_error("invalid ai::Action passed as an argument");
		Move move = *movePtr;

		addPiece(move.col);
	}

	bool Board::isTerminal() const {
		return checkForWin() != 0;
	}

}