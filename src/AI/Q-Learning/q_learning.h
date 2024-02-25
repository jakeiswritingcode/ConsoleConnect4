// q_learning.h
// by Jake Charles Osborne III
#pragma once



#include "qlstate.h"
#include "Function Approximators/function_approximators.h"
#include "Policies/policies.h"



namespace ai {

	enum class Policy { EpsilonGreedy, Softmax };

	void trainModel(QLState& initialState, RegressionModel& model, int episodes, Policy policy, float policyModifier, float learningRate, float discountFactor);

	void trainModel(SharedQLState& initialState, RegressionModel& model, int episodes, Policy policy, float policyModifier, float learningRate, float discountFactor);

}