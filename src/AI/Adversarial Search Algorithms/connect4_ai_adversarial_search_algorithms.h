// connect4_ai_adversarial_search_algorithms.h
// by Jake Charles Osborne III
#pragma once



#include "../../MVC/Model/connect4_model.h"



namespace connect4::ai {

	struct MinimaxResult {
		short moveIndex;
		double moveScore;
	};

	MinimaxResult minimax(const connect4::model::Board&, short maximizingPlayer, int depth);
	MinimaxResult multithreadingMinimax(const connect4::model::Board& board, short maximizingPlayer, int depth);

	MinimaxResult negamax(const connect4::model::Board&, short maximizingPlayer, int depth);
	MinimaxResult multithreadingNegamax(const connect4::model::Board& board, short maximizingPlayer, int depth);

	MinimaxResult alphaBeta(const connect4::model::Board&, short maximizingPlayer, int depth);
	MinimaxResult multithreadingAlphaBeta(const connect4::model::Board&, short maximizingPlayer, int depth);

}