// q_learning.cpp
// by Jake Charles Osborne III



#include "q_learning.h"
#include "qlstate.h"
#include "Function Approximators/function_approximators.h"
#include "Action Selection/action_selection_algorithms.h"
#include <vector>
#include <memory>
#include <alg.h>

using std::shared_ptr;



namespace ai {

    void trainModel(QLState& initialState, RegressionModel& model, int episodes, float epsilon, float learningRate, float discountFactor) {
        for (int e = 0; e < episodes; ++e) {
            shared_ptr<QLState> state = initialState.cloneQLState();
            while (!state->isTerminal()) {
                std::vector<float> features = state->extractFeatures();
                float qVal = model.predict(features);

                double reward = state->useAction(
                    getEpsilonGreedyAction(*state, model, epsilon));

                float nextQMax = 0;
                if (!state->isTerminal()) {
                    nextQMax = -FLT_MAX;
                    for (auto& action : state->getActions()) {
                        shared_ptr<QLState> newState = state->cloneQLState();
                        newState->useAction(action);
                        float nextQ = model.predict(newState->extractFeatures());
                        if (nextQ > nextQMax) {
                            nextQMax = nextQ;
                        }
                    }
                }

                float tdTarget = reward + discountFactor * nextQMax;
                model.updateWeights(features, learningRate, tdTarget);
            }
        }
    }

}