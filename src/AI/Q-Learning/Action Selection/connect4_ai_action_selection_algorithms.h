// connect4_ai_action_selection_algorithms.h
// by Jake Charles Osborne III



#include "../Function Approximators/connect4_ai_function_approximators.h"
#include "../../../MVC/Model/connect4_model.h"

#include <vector>



namespace connect4::ai {

    std::shared_ptr<Action> getEpsilonGreedyAction(const ai::State&, const RegressionModel&, float epsilon = 0.1f);

    std::shared_ptr<Action> getSoftmaxAction(const ai::State&, const RegressionModel&, float beta = 1);

}