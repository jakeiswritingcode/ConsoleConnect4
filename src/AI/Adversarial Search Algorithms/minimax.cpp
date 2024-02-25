// minimax.cpp
// by Jake Charles Osborne III



#include "adversarial_search_algorithms.h"
#include <memory>
#include <vector>
#include <future>

using std::shared_ptr;
using std::vector;
using std::future;
using std::async;



namespace {

    struct MinimaxResult {
        shared_ptr<ai::Action> move;
        double score;
    };

}

namespace {

    MinimaxResult minimax(const ai::ASAState& state, short maximizingAgentId, int depth);

    MinimaxResult minimize(const ai::ASAState& state, short maximizingAgentId, int depth) {
        MinimaxResult bestResult = { nullptr, DBL_MAX };

        for (auto& action : state.getActions()) {
            auto nextState = state.cloneASAState();
            nextState->useAction(action);

            double score = ::minimax(*nextState, maximizingAgentId, depth - 1).score;
            if (score < bestResult.score) bestResult = { action, score };
        }

        return bestResult;
    }

    MinimaxResult maximize(const ai::ASAState& state, short maximizingAgentId, int depth) {
        MinimaxResult bestResult = { nullptr, -DBL_MAX };

        for (auto& action : state.getActions()) {
            auto nextState = state.cloneASAState();
            nextState->useAction(action);

            double score = ::minimax(*nextState, maximizingAgentId, depth - 1).score;
            if (score > bestResult.score) bestResult = { action, score };
        }

        return bestResult;
    }

    MinimaxResult minimax(const ai::ASAState& state, short maximizingAgentId, int depth) {
        if (state.isTerminal() || depth <= 0) return { nullptr, state.evaluate(maximizingAgentId) };

        if (maximizingAgentId == state.getActiveASAAgentId()) {
            return maximize(state, maximizingAgentId, depth);
        }
        else {
            return minimize(state, maximizingAgentId, depth);
        }
    }

}

namespace {

    MinimaxResult multithreadingMinimax(shared_ptr<ai::ASAState> state, short maximizingAgentId, int depth);

    MinimaxResult multithreadingMinimize(shared_ptr<ai::ASAState> state, short maximizingAgentId, int depth) {
        MinimaxResult bestResult = { nullptr, DBL_MAX };

        auto actions = state->getActions();
        vector<future<MinimaxResult>> futures;
        futures.reserve(actions.size());
        for (int i = 0; i < actions.size(); ++i) {
            auto nextState = state->cloneASAState();
            nextState->useAction(actions[i]);
            futures.emplace_back(async(std::launch::async,
                ::multithreadingMinimax, nextState, maximizingAgentId, depth - 1));
        }
        for (int i = 0; i < futures.size(); ++i) {
            double score = futures[i].get().score;
            if (score < bestResult.score) bestResult = { actions[i], score };
        }

        return bestResult;
    }

    MinimaxResult multithreadingMaximize(shared_ptr<ai::ASAState> state, short maximizingAgentId, int depth) {
        MinimaxResult bestResult = { nullptr, -DBL_MAX };

        auto actions = state->getActions();
        vector<future<MinimaxResult>> futures;
        futures.reserve(actions.size());
        for (int i = 0; i < actions.size(); ++i) {
            auto nextState = state->cloneASAState();
            nextState->useAction(actions[i]);
            futures.emplace_back(async(std::launch::async,
                ::multithreadingMinimax, nextState, maximizingAgentId, depth - 1));
        }
        for (int i = 0; i < futures.size(); ++i) {
            double score = futures[i].get().score;
            if (score > bestResult.score) bestResult = { actions[i], score };
        }

        return bestResult;
    }

    MinimaxResult multithreadingMinimax(shared_ptr<ai::ASAState> state, short maximizingAgentId, int depth) {
        if (state->isTerminal() || depth <= 0) return { nullptr, state->evaluate(maximizingAgentId) };

        if (maximizingAgentId == state->getActiveASAAgentId()) {
            return multithreadingMaximize(state, maximizingAgentId, depth);
        }
        else {
            return multithreadingMinimize(state, maximizingAgentId, depth);
        }
    }

}

namespace ai {

    shared_ptr<Action> minimax(const ASAState& state, int depth) {
        return ::minimax(state, state.getActiveASAAgentId(), depth).move;
    }

    shared_ptr<Action> multithreadingMinimax(const ASAState& state, int depth) {
        return ::multithreadingMinimax(state.cloneASAState(), state.getActiveASAAgentId(), depth).move;
    }

}