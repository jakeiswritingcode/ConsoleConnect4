// connect4_model_evaluation.h
// by Jake Charles Osborne III
#pragma once



#include <bitset>



namespace connect4::model {

	std::bitset<47> getEmptySpaces(const std::bitset<47>& minPlayer, const std::bitset<47>& maxPlayer);
	int countTwos(const std::bitset<47>& player, const std::bitset<47>& emptySpaces);
	int countThrees(const std::bitset<47>& player, const std::bitset<47>& emptySpaces);

}