// connect4_model_qlstate.cpp
// by Jake Charles Osborne III



#include "connect4_model.h"
#include "Evaluation/connect4_model_evaluation.h"
#include <vector>
#include <bitset>
#include <stdexcept>

using std::vector;
using std::bitset;



namespace connect4::model {
	
	vector<float> Board::extractFeatures() const {
		const bitset<47>& activePlayer = (getActivePlayer() == 1) ? player1 : player2;
		const bitset<47>& inactivePlayer = (getActivePlayer() == 1) ? player2 : player1;
		bitset<47> emptySpaces = getEmptySpaces(activePlayer, inactivePlayer);

		return {
			float(countTwos(activePlayer, emptySpaces)),
			float(countThrees(activePlayer, emptySpaces)),
			float(countTwos(inactivePlayer, emptySpaces)),
			float(countThrees(inactivePlayer, emptySpaces))
		};
	}

}