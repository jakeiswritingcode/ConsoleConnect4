// connect4_ai_q_learning.cpp
// by Jake Charles Osborne III


#include "connect4_ai_q_learning.h"
#include "state_action_interface.h"
#include "../../MVC/Model/connect4_model.h"
#include "Function Approximators/connect4_ai_function_approximators.h"
#include "Action Selection/connect4_ai_action_selection_algorithms.h"
#include "Evaluation/connect4_ai_q_learning_evaluation.h"

using namespace connect4;

#include <vector>

using std::vector;



namespace {

    void trainAI(ai::State& state, ai::LinearRegression& model, float learningRate = 0.05f) {
        while (!state.isTerminal()) {

            // Use an epsilon-greedy policy to choose an action
            ai::Action action = getEpsilonGreedyAction(state, model, .1);

            // Apply the action to get the new state and reward
            model::State newState = state;
            newState.addPiece(action);

            float reward = getReward(newState);

            // Update the model
            vector<float> newStateFeatures = extractFeatures(newState);
            float bestFutureValue = estimateBestFutureValue(newState, model);
            float target = reward + bestFutureValue;
            model.updateWeights(state.extractFeatures(), target, learningRate);

            // Update the board state
            board = newState;
        }
    }

}

namespace connect4::ai {

    int qLearning(model::Board& board, ai::LinearRegression& model) {
        // Observing the current state of the board.
        
    
        // Extracting features from the current state.
        vector<float> features = extractFeatures(board);
        
        // Using getEpsilonGreedyAction to decide on the next move.
        
        
        // Applying the move to get to the next state.
        
        
        // Observing the reward and the next state.
        
        
        // Updating the LinearRegression model using the observed reward and the difference in predicted values between the current and next states, adjusted by a discount factor if you're implementing Q-learning.
    
    
    }

}