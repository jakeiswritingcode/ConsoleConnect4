// connect4_model_evaluation.cpp
// by Jake Charles Osborne III



#include "connect4_model_evaluation.h"
#include <bitset>

using std::bitset;



namespace {

	int countTwos(const bitset<47>& player, const bitset<47>& emptySpaces, int shift1, int shift2, int shift3) {
		return (emptySpaces & (emptySpaces << shift1) & (player << shift2) & (player << shift3)).count() +
			(emptySpaces & (player << shift1) & (emptySpaces << shift2) & (player << shift3)).count() +
			(emptySpaces & (player << shift1) & (player << shift2) & (emptySpaces << shift3)).count() +
			(player & (emptySpaces << shift1) & (emptySpaces << shift2) & (player << shift3)).count() +
			(player & (emptySpaces << shift1) & (player << shift2) & (emptySpaces << shift3)).count() +
			(player & (player << shift1) & (emptySpaces << shift2) & (emptySpaces << shift3)).count();
	}

	int countThrees(const bitset<47>& player, const bitset<47>& emptySpaces, int shift1, int shift2, int shift3) {
		return (emptySpaces & (player << shift1) & (player << shift2) & (player << shift3)).count() +
			(player & (emptySpaces << shift1) & (player << shift2) & (player << shift3)).count() +
			(player & (player << shift1) & (emptySpaces << shift2) & (player << shift3)).count() +
			(player & (player << shift1) & (player << shift2) & (emptySpaces << shift3)).count();
	}

}

namespace connect4::model {

	bitset<47> getEmptySpaces(const bitset<47>& minPlayer, const bitset<47>& maxPlayer) {
		bitset<47> emptySpaces = ~(minPlayer | maxPlayer);
		emptySpaces.reset(7);
		emptySpaces.reset(15);
		emptySpaces.reset(23);
		emptySpaces.reset(31);
		emptySpaces.reset(39);

		return emptySpaces;
	}

	int countTwos(const bitset<47>& player, const bitset<47>& emptySpaces) {
		return
			::countTwos(player, emptySpaces, 1, 2, 3) +   // Horizontal
			::countTwos(player, emptySpaces, 8, 16, 24) + // Vertical
			::countTwos(player, emptySpaces, 7, 14, 21) + // Diagonal
			::countTwos(player, emptySpaces, 9, 18, 27);  // Anti-diagonal
	}

	int countThrees(const bitset<47>& player, const bitset<47>& emptySpaces) {
		return
			::countThrees(player, emptySpaces, 1, 2, 3) +   // Horizontal
			::countThrees(player, emptySpaces, 8, 16, 24) + // Vertical
			::countThrees(player, emptySpaces, 7, 14, 21) + // Diagonal
			::countThrees(player, emptySpaces, 9, 18, 27);  // Anti-diagonal
	}

}