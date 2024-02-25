// adversarial_search_algorithms.h
// by Jake Charles Osborne III
#pragma once



#include "asastate.h"
#include <memory>



namespace ai {

	std::shared_ptr<Action> minimax(const ai::ASAState&, int depth);
	std::shared_ptr<Action> multithreadingMinimax(const ai::ASAState&, int depth);

	std::shared_ptr<Action> negamax(const ai::ASAState&, int depth);
	std::shared_ptr<Action> multithreadingNegamax(const ai::ASAState&, int depth);

	std::shared_ptr<Action> alphaBeta(const ai::ASAState&, int depth);
	std::shared_ptr<Action> multithreadingAlphaBeta(const ai::ASAState&, int depth);

}