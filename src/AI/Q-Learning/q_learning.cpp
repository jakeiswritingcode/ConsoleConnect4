// q_learning.cpp
// by Jake Charles Osborne III



#include "q_learning.h"
#include "qlstate.h"
#include "Function Approximators/function_approximators.h"
#include "Action Selection/action_selection_algorithms.h"
#include <memory>

using std::shared_ptr;



namespace ai {

    void trainModel(QLState& initialState, RegressionModel& model, int episodes, float epsilon, float learningRate, float discountFactor) {
        for (int e = 0; e < episodes; ++e) {
            shared_ptr<QLState> state = state->cloneQLState();
            while (!state->isTerminal()) {
                shared_ptr<Action> action = getEpsilonGreedyAction(*state, model, epsilon);
                shared_ptr<QLState> oldState = state->cloneQLState();
                double reward = state->useAction(action);

                // Update
                float qOld = model.predict(oldState->extractFeatures());
                float qNewMax = -FLT_MAX;
                for (auto& action : state->getActions()) {
                    shared_ptr<QLState> newState = state->cloneQLState();
                    newState->useAction(action);
                    float qValue = model.predict(newState->extractFeatures());
                    if (qValue > qNewMax) {
                        qNewMax = qValue;
                    }
                }

                // TODO: Double check the functionality in this method

                float qTarget = reward + discountFactor * qNewMax;
                model.updateWeights(oldState->extractFeatures(), qTarget, learningRate);
            }
        }
    }

}