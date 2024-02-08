// connect4_ai_evaluation.cpp
// by Jake Charles Osborne III


#include "connect4_ai_evaluation.h";
#include "../../MVC/Model/connect4_model.h"



namespace {

	int countThrees(const std::bitset<47>& player, const std::bitset<47>& emptySpaces, int shift1, int shift2, int shift3) {
		return (emptySpaces & (player << shift1) & (player << shift2) & (player << shift3)).count() +
			(player & (emptySpaces << shift1) & (player << shift2) & (player << shift3)).count() +
			(player & (player << shift1) & (emptySpaces << shift2) & (player << shift3)).count() +
			(player & (player << shift1) & (player << shift2) & (emptySpaces << shift3)).count();
	}

	int countTwos(const std::bitset<47>& player, const std::bitset<47>& emptySpaces, int shift1, int shift2, int shift3) {
		return (emptySpaces & (emptySpaces << shift1) & (player << shift2) & (player << shift3)).count() +
			(emptySpaces & (player << shift1) & (emptySpaces << shift2) & (player << shift3)).count() +
			(emptySpaces & (player << shift1) & (player << shift2) & (emptySpaces << shift3)).count() +
			(player & (emptySpaces << shift1) & (emptySpaces << shift2) & (player << shift3)).count() +
			(player & (emptySpaces << shift1) & (player << shift2) & (emptySpaces << shift3)).count() +
			(player & (player << shift1) & (emptySpaces << shift2) & (emptySpaces << shift3)).count();
	}

}

namespace connect4::ai {

	double evaluate(const connect4::model::Board& board, short maximizingPlayer) {
		const std::bitset<47>& minPlayer = (maximizingPlayer == 1) ? board.player2 : board.player1;
		const std::bitset<47>& maxPlayer = (maximizingPlayer == 1) ? board.player1 : board.player2;
		std::bitset<47> emptySpaces = ~(minPlayer | maxPlayer);
		emptySpaces.reset(7);
		emptySpaces.reset(15);
		emptySpaces.reset(23);
		emptySpaces.reset(31);
		emptySpaces.reset(39);

		int maximizerThrees = countThrees(maxPlayer, emptySpaces, 1, 2, 3) // Horizontal
							- countThrees(minPlayer, emptySpaces, 1, 2, 3)
							+ countThrees(maxPlayer, emptySpaces, 8, 16, 24) // Vertical
							- countThrees(minPlayer, emptySpaces, 8, 16, 24)
							+ countThrees(maxPlayer, emptySpaces, 7, 14, 21) // Diagonal
							- countThrees(minPlayer, emptySpaces, 7, 14, 21)
							+ countThrees(maxPlayer, emptySpaces, 9, 18, 27) // Anti-diagonal
							- countThrees(minPlayer, emptySpaces, 9, 18, 27);

		int maximizerTwos	= countTwos(maxPlayer, emptySpaces, 1, 2, 3) // Horizontal
							- countTwos(minPlayer, emptySpaces, 1, 2, 3)
							+ countTwos(maxPlayer, emptySpaces, 8, 16, 24) // Vertical
							- countTwos(minPlayer, emptySpaces, 8, 16, 24)
							+ countTwos(maxPlayer, emptySpaces, 7, 14, 21) // Diagonal
							- countTwos(minPlayer, emptySpaces, 7, 14, 21)
							+ countTwos(maxPlayer, emptySpaces, 9, 18, 27) // Anti-diagonal
							- countTwos(minPlayer, emptySpaces, 9, 18, 27);

		return maximizerThrees * 10 + maximizerTwos * 3;
	}

}