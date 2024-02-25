// epsilongreedy.cpp
// by Jake Charles Osborne III



#include "policies.h"
#include "../qlstate.h"
#include <random>
#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;



namespace {

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

}

namespace ai {

    shared_ptr<Action> getEpsilonGreedyAction(const QLState& state, const RegressionModel& model, float epsilon) {
        vector<shared_ptr<Action>> actions = state.getActions();
        if (actions.empty()) throw std::logic_error("State with no available Actions passed as argument to selection algorithm");

        // Exploration: Choose a random action with probability epsilon
        std::uniform_real_distribution<> epsilonDis(0.0, 1.0);
        if (epsilonDis(gen) < epsilon) {
            std::uniform_int_distribution<> actionDis(0, actions.size() - 1);
            int randomAction = actionDis(gen);

            return actions[randomAction];
        }

        // Exploitation: Choose the best action based on the current model
        float bestValue = -FLT_MAX;
        shared_ptr<Action> bestAction;

        for (auto& action : actions) {
            shared_ptr<QLState> newState = state.cloneQLState();
            newState->takeAction(action);
            float value = model.predict(newState->extractFeatures());

            if (value > bestValue) {
                bestValue = value;
                bestAction = action;
            }
        }

        return bestAction;
    }

    shared_ptr<Action> getEpsilonGreedyAction(const SharedQLState& state, const RegressionModel& model, float epsilon) {
        vector<shared_ptr<Action>> actions = state.getActions();
        if (actions.empty()) throw std::logic_error("State with no available Actions passed as argument to selection algorithm");

        // Exploration: Choose a random action with probability epsilon
        std::uniform_real_distribution<> epsilonDis(0.0, 1.0);
        if (epsilonDis(gen) < epsilon) {
            std::uniform_int_distribution<> actionDis(0, actions.size() - 1);
            int randomAction = actionDis(gen);

            return actions[randomAction];
        }

        // Exploitation: Choose the best action based on the current model
        float bestValue = -FLT_MAX;
        shared_ptr<Action> bestAction;

        for (auto& action : actions) {
            shared_ptr<SharedQLState> newState = state.cloneSharedQLState();
            newState->takeAction(action);
            float value = model.predict(newState->extractFeatures(state.getActiveQLAgentId()));

            if (value > bestValue) {
                bestValue = value;
                bestAction = action;
            }
        }

        return bestAction;
    }

}