// q_learning.cpp
// by Jake Charles Osborne III



#include "q_learning.h"
#include "qlstate.h"
#include "Function Approximators/function_approximators.h"
#include "Policies/policies.h"
#include <vector>
#include <memory>
#include <stdexcept>

using namespace ai;
using std::shared_ptr;



namespace {
    
    shared_ptr<Action> selectAction(QLState& state, RegressionModel& model, Policy policy, float policyModifier) {
        switch (policy) {
        case Policy::EpsilonGreedy:
            return getEpsilonGreedyAction(state, model, policyModifier);
        case Policy::Softmax:
            return getSoftmaxAction(state, model, policyModifier);
        }

        throw std::logic_error("inavlid ai::SelectionAlgorithm passed as argument");
    }

    shared_ptr<Action> selectAction(SharedQLState& state, RegressionModel& model, Policy policy, float policyModifier) {
        switch (policy) {
        case Policy::EpsilonGreedy:
            return getEpsilonGreedyAction(state, model, policyModifier);
        case Policy::Softmax:
            return getSoftmaxAction(state, model, policyModifier);
        }

        throw std::logic_error("inavlid ai::SelectionAlgorithm passed as argument");
    }

}

namespace ai {

    void trainModel(QLState& initialState, RegressionModel& model, int episodes, Policy policy, float policyModifier, float learningRate, float discountFactor) {
        for (int e = 0; e < episodes; ++e) {
            shared_ptr<QLState> state = initialState.cloneQLState();
            while (!state->isTerminal()) {
                std::vector<float> features = state->extractFeatures();
                float qVal = model.predict(features);

                shared_ptr<Action> action = ::selectAction(*state, model, policy, policyModifier);
                double reward = state->takeAction(action);

                float nextQMax;
                if (!state->isTerminal()) {
                    nextQMax = -FLT_MAX;
                    for (auto& nextAction : state->getActions()) {
                        shared_ptr<QLState> newState = state->cloneQLState();
                        newState->takeAction(nextAction);
                        float nextQ = model.predict(newState->extractFeatures());
                        if (nextQ > nextQMax) {
                            nextQMax = nextQ;
                        }
                    }
                }
                else {
                    nextQMax = 0;
                }

                float tdTarget = reward + discountFactor * nextQMax;
                model.updateWeights(features, learningRate, tdTarget);
            }
        }
    }

    void trainModel(SharedQLState& initialState, RegressionModel& model, int episodes, Policy policy, float policyModifier, float learningRate, float discountFactor) {
        for (int e = 0; e < episodes; ++e) {
            shared_ptr<SharedQLState> state = initialState.cloneSharedQLState();
            while (!state->isTerminal()) {
                short activeAgentId = initialState.getActiveQLAgentId();
                std::vector<float> features = state->extractFeatures(activeAgentId);
                float qVal = model.predict(features);

                auto action = ::selectAction(*state, model, policy, policyModifier);
                double reward = state->takeAction(action);

                float nextQMax;
                if (!state->isTerminal()) {
                    nextQMax = -FLT_MAX;
                    for (auto& nextAction : state->getActions()) {
                        shared_ptr<SharedQLState> newState = state->cloneSharedQLState();
                        newState->takeAction(nextAction);
                        float nextQ = model.predict(newState->extractFeatures(activeAgentId));
                        if (nextQ > nextQMax) {
                            nextQMax = nextQ;
                        }
                    }
                }
                else {
                    nextQMax = 0;
                }

                float tdTarget = reward + discountFactor * nextQMax;
                model.updateWeights(features, learningRate, tdTarget);
            }
        }
    }

}