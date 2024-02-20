// connect4_ai_evaluation.cpp
// by Jake Charles Osborne III


#include "connect4_ai_evaluation.h"
#include "../../../MVC/Model/connect4_model.h"

using std::bitset;



namespace {

	bitset<47> getEmptySpaces(const bitset<47>& minPlayer, const bitset<47>& maxPlayer) {
		bitset<47> emptySpaces = ~(minPlayer | maxPlayer);
		emptySpaces.reset(7);
		emptySpaces.reset(15);
		emptySpaces.reset(23);
		emptySpaces.reset(31);
		emptySpaces.reset(39);

		return emptySpaces;
	}

	int countThrees(const bitset<47>& player, const bitset<47>& emptySpaces, int shift1, int shift2, int shift3) {
		return (emptySpaces & (player << shift1) & (player << shift2) & (player << shift3)).count() +
			(player & (emptySpaces << shift1) & (player << shift2) & (player << shift3)).count() +
			(player & (player << shift1) & (emptySpaces << shift2) & (player << shift3)).count() +
			(player & (player << shift1) & (player << shift2) & (emptySpaces << shift3)).count();
	}

	int countThrees(const bitset<47>& player, const bitset<47>& emptySpaces) {
		return
			countThrees(player, emptySpaces, 1, 2, 3) +   // Horizontal
			countThrees(player, emptySpaces, 8, 16, 24) + // Vertical
			countThrees(player, emptySpaces, 7, 14, 21) + // Diagonal
			countThrees(player, emptySpaces, 9, 18, 27);  // Anti-diagonal
	}

	int countTwos(const bitset<47>& player, const bitset<47>& emptySpaces, int shift1, int shift2, int shift3) {
		return (emptySpaces & (emptySpaces << shift1) & (player << shift2) & (player << shift3)).count() +
			(emptySpaces & (player << shift1) & (emptySpaces << shift2) & (player << shift3)).count() +
			(emptySpaces & (player << shift1) & (player << shift2) & (emptySpaces << shift3)).count() +
			(player & (emptySpaces << shift1) & (emptySpaces << shift2) & (player << shift3)).count() +
			(player & (emptySpaces << shift1) & (player << shift2) & (emptySpaces << shift3)).count() +
			(player & (player << shift1) & (emptySpaces << shift2) & (emptySpaces << shift3)).count();
	}

	int countTwos(const bitset<47>& player, const bitset<47>& emptySpaces) {
		return
			countTwos(player, emptySpaces, 1, 2, 3) +   // Horizontal
			countTwos(player, emptySpaces, 8, 16, 24) + // Vertical
			countTwos(player, emptySpaces, 7, 14, 21) + // Diagonal
			countTwos(player, emptySpaces, 9, 18, 27);  // Anti-diagonal
	}

}

namespace connect4::ai {

	double evaluate(const connect4::model::Board& board, short maximizingPlayer) {
		const bitset<47>& minPlayer = (maximizingPlayer == 1) ? board.player2 : board.player1;
		const bitset<47>& maxPlayer = (maximizingPlayer == 1) ? board.player1 : board.player2;
		bitset<47> emptySpaces = getEmptySpaces(minPlayer, maxPlayer);

		int maximizerThrees = countThrees(maxPlayer, emptySpaces);
		int minimizerThrees = countThrees(minPlayer, emptySpaces);
		int maximizerTwos = countTwos(maxPlayer, emptySpaces);
		int minimizerTwos = countTwos(minPlayer, emptySpaces);

		return (maximizerThrees - minimizerThrees) * 10 + (maximizerTwos - minimizerTwos)* 3;
	}

}