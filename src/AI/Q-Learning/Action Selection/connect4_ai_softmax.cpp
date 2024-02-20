// connect4_ai_softmax.cpp
// by Jake Charles Osborne III



#include "connect4_ai_action_selection_algorithms.h"
#include "../../../MVC/Model/connect4_model.h"
#include "../Evaluation/connect4_ai_q_learning_evaluation.h"

#include <random>
#include <vector>

using std::vector;
using std::shared_ptr;


namespace {

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

}

namespace connect4::ai {

    Action getSoftmaxAction(const ai::State& state, const RegressionModel& model, float beta) {
        vector<shared_ptr<Action>> actions = state.getActions();
        if (actions.empty()) throw std::logic_error("State with no available Actions passed as argument to selection algorithm");

        vector<float> actionValues;
        for (auto& action : actions) {
            shared_ptr<State> newState = state.clone();
            newState->useAction(action);

            float value = model.predict(newState->extractFeatures());
            actionValues.push_back(value);
        }

        vector<float> actionProbabilities = softmax(actionValues, beta);
        int actionIndex = sampleAction(actionProbabilities);

        return actionIndex;
    }

    vector<float> softmax(const vector<float>& values, float beta) {
        vector<float> expValues(values.size());
        float sumExpValues = 0.0;

        // Compute e^(beta * value) for each value and their sum
        for (int i = 0; i < values.size(); ++i) {
            expValues[i] = std::exp(beta * values[i]);
            sumExpValues += expValues[i];
        }

        // Normalize to get probabilities
        for (float& val : expValues) val /= sumExpValues;

        return expValues;
    }

    Action sampleAction(const vector<float>& probabilities) {
        std::uniform_real_distribution<> dis(0.0, 1.0);
        float sample = dis(gen);

        float sum = 0.0;
        for (int i = 0; i < probabilities.size(); ++i) {
            sum += probabilities[i];
            if (sample <= sum) {
                return i;
            }
        }

        return probabilities.size() - 1; // Should not be reached
    }

}