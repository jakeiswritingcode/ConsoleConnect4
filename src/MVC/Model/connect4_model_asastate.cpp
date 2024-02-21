// connect4_model_asastate.cpp
// by Jake Charles Osborne III



#include "connect4_model.h"
#include "Evaluation/connect4_model_evaluation.h"
#include <bitset>
#include <stdexcept>

using std::bitset;



namespace connect4::model {

    short Board::getActivePlayerId() const {
        return getActivePlayer();
    }

    double Board::evaluate(short maximizingPlayerId) const {
		if (maximizingPlayerId != 1 && maximizingPlayerId != 2) throw std::logic_error("invalid maximizingPlayerId passed as argument");

        short winner = checkForWin();
        if (winner == maximizingPlayerId) return DBL_MAX;
        if (winner == 1 || winner == 2) return -DBL_MAX;
        if (winner == -1) return 0; // tie

		const bitset<47>& maxPlayer = (maximizingPlayerId == 1) ? player1 : player2;
		const bitset<47>& minPlayer = (maximizingPlayerId == 1) ? player2 : player1;
		const bitset<47>& emptySpaces = getEmptySpaces(minPlayer, maxPlayer);

		int maximizerThrees = countThrees(maxPlayer, emptySpaces);
		int minimizerThrees = countThrees(minPlayer, emptySpaces);
		int maximizerTwos = countTwos(maxPlayer, emptySpaces);
		int minimizerTwos = countTwos(minPlayer, emptySpaces);

		return (maximizerThrees - minimizerThrees) * 10 + (maximizerTwos - minimizerTwos) * 3;
	}

}