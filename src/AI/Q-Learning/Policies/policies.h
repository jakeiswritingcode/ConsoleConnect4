// policies.h
// by Jake Charles Osborne III
#pragma once



#include "../qlstate.h"
#include "../Function Approximators/function_approximators.h"
#include <memory>



namespace ai {

    std::shared_ptr<Action> getEpsilonGreedyAction(const QLState&, const RegressionModel&, float epsilon = 0.1f);
    std::shared_ptr<Action> getEpsilonGreedyAction(const SharedQLState&, const RegressionModel&, float epsilon = 0.1f);

    std::shared_ptr<Action> getSoftmaxAction(const QLState&, const RegressionModel&, float beta = 1.0f);
    std::shared_ptr<Action> getSoftmaxAction(const SharedQLState&, const RegressionModel&, float beta = 1.0f);

}