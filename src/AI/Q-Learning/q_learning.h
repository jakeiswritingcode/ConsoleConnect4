// q_learning.h
// by Jake Charles Osborne III
#pragma once



#include "qlstate.h"
#include "Policies/policies.h"
#include "Function Approximators/function_approximators.h"



namespace ai {

	void trainModel(QLState& initialState, RegressionModel& model, int episodes, float epsilon, float learningRate, float discountFactor);
	
}