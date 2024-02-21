// softmax.cpp
// by Jake Charles Osborne III



#include "action_selection_algorithms.h"
#include <random>
#include <vector>

using std::vector;
using std::shared_ptr;



namespace {

    // random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

}

namespace ai {

    shared_ptr<Action> getSoftmaxAction(const State& state, const RegressionModel& model, float beta) {
        vector<shared_ptr<Action>> actions = state.getActions();
        if (actions.empty()) throw std::logic_error("State with no available Actions passed as argument to selection algorithm");

        vector<float> probabilities(actions.size());
        for (size_t i = 0; i < actions.size(); ++i) {
            shared_ptr<State> newState = state.clone();
            newState->useAction(actions[i]);

            float value = model.predict(newState->extractFeatures());
            probabilities[i] = std::exp(beta * value);
        }

        std::discrete_distribution<size_t> distribution(probabilities.begin(), probabilities.end());
        size_t actionIndex = distribution(gen);
        return actions[actionIndex];
    }

}