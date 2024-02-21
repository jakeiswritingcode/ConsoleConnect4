// q_learning.h
// by Jake Charles Osborne III
#pragma once



#include "qlstate.h"
#include "Action Selection/action_selection_algorithms.h"
#include "Function Approximators/function_approximators.h"




namespace ai {

	void trainModel(QLState& initialState, RegressionModel& model, int episodes, float epsilon, float learningRate, float discountFactor);
	
}