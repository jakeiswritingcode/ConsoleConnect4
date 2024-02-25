// softmax.cpp
// by Jake Charles Osborne III



#include "policies.h"
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

    shared_ptr<Action> getSoftmaxAction(const QLState& state, const RegressionModel& model, float beta) {
        vector<shared_ptr<Action>> actions = state.getActions();
        if (actions.empty()) throw std::logic_error("State with no available Actions passed as argument to selection algorithm");

        vector<float> probabilities(actions.size());
        for (size_t i = 0; i < actions.size(); ++i) {
            shared_ptr<QLState> newState = state.cloneQLState();
            newState->takeAction(actions[i]);

            float value = model.predict(newState->extractFeatures());
            probabilities[i] = std::exp(beta * value);
        }

        std::discrete_distribution<size_t> distribution(probabilities.begin(), probabilities.end());
        size_t actionIndex = distribution(gen);
        return actions[actionIndex];
    }

    shared_ptr<Action> getSoftmaxAction(const SharedQLState& state, const RegressionModel& model, float beta) {
        vector<shared_ptr<Action>> actions = state.getActions();
        if (actions.empty()) throw std::logic_error("State with no available Actions passed as argument to selection algorithm");

        vector<float> probabilities(actions.size());
        for (size_t i = 0; i < actions.size(); ++i) {
            shared_ptr<SharedQLState> newState = state.cloneSharedQLState();
            newState->takeAction(actions[i]);

            float value = model.predict(newState->extractFeatures(state.getActiveQLAgentId()));
            probabilities[i] = std::exp(beta * value);
        }

        std::discrete_distribution<size_t> distribution(probabilities.begin(), probabilities.end());
        size_t actionIndex = distribution(gen);
        return actions[actionIndex];
    }

}