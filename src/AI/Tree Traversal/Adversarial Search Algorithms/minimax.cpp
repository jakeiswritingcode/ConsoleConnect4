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
        double moveScore;
    };

}

namespace {

    MinimaxResult minimax(const ai::ASAState& state, short maximizingPlayerId, int depth);

    MinimaxResult minimize(const ai::ASAState& state, short maximizingPlayerId, int depth) {
        MinimaxResult bestResult = { nullptr, DBL_MAX };

        for (auto& action : state.getActions()) {
            auto nextState = state.cloneASAState();
            nextState->useAction(action);

            double score = ::minimax(*nextState, maximizingPlayerId, depth - 1).moveScore;
            if (score < bestResult.moveScore) bestResult = { action, score };
        }

        return bestResult;
    }

    MinimaxResult maximize(const ai::ASAState& state, short maximizingPlayerId, int depth) {
        MinimaxResult bestResult = { nullptr, -DBL_MAX };

        for (auto& action : state.getActions()) {
            auto nextState = state.cloneASAState();
            nextState->useAction(action);

            double score = ::minimax(*nextState, maximizingPlayerId, depth - 1).moveScore;
            if (score > bestResult.moveScore) bestResult = { action, score };
        }

        return bestResult;
    }

    MinimaxResult minimax(const ai::ASAState& state, short maximizingPlayerId, int depth) {
        if (state.isTerminal() || depth <= 0) return { nullptr, state.evaluate(maximizingPlayerId) };

        if (maximizingPlayerId == state.getActivePlayerId()) {
            return maximize(state, maximizingPlayerId, depth);
        }
        else {
            return minimize(state, maximizingPlayerId, depth);
        }
    }

}

namespace {

    MinimaxResult multithreadingMinimax(const ai::ASAState& state, short maximizingPlayerId, int depth);

    MinimaxResult multithreadingMinimize(const ai::ASAState& state, short maximizingPlayerId, int depth) {
        MinimaxResult bestResult = { nullptr, DBL_MAX };

        auto actions = state.getActions();
        vector<future<MinimaxResult>> futures;
        futures.reserve(actions.size());
        for (int i = 0; i < actions.size(); ++i) {
            auto nextState = state.cloneASAState();
            nextState->useAction(actions[i]);
            futures.emplace_back(async(std::launch::async,
                ::multithreadingMinimax, *nextState, maximizingPlayerId, depth - 1));
        }
        for (int i = 0; i < futures.size(); ++i) {
            double score = futures[i].get().moveScore;
            if (score < bestResult.moveScore) bestResult = { actions[i], score };
        }

        return bestResult;
    }

    MinimaxResult multithreadingMaximize(const ai::ASAState& state, short maximizingPlayerId, int depth) {
        MinimaxResult bestResult = { nullptr, -DBL_MAX };

        auto actions = state.getActions();
        vector<future<MinimaxResult>> futures;
        futures.reserve(actions.size());
        for (int i = 0; i < actions.size(); ++i) {
            auto nextState = state.cloneASAState();
            nextState->useAction(actions[i]);
            futures.emplace_back(async(std::launch::async,
                ::multithreadingMinimax, *nextState, maximizingPlayerId, depth - 1));
        }
        for (int i = 0; i < futures.size(); ++i) {
            double score = futures[i].get().moveScore;
            if (score > bestResult.moveScore) bestResult = { actions[i], score };
        }

        return bestResult;
    }

    MinimaxResult multithreadingMinimax(const ai::ASAState& state, short maximizingPlayerId, int depth) {
        if (state.isTerminal() || depth <= 0) return { nullptr, state.evaluate(maximizingPlayerId) };

        if (maximizingPlayerId == state.getActivePlayerId()) {
            return multithreadingMaximize(state, maximizingPlayerId, depth);
        }
        else {
            return multithreadingMinimize(state, maximizingPlayerId, depth);
        }
    }

}

namespace ai {

    shared_ptr<Action> minimax(const ai::ASAState& state, int depth) {
        return ::minimax(state, state.getActivePlayerId(), depth).move;
    }

    shared_ptr<Action> multithreadingMinimax(const ai::ASAState& state, int depth) {
        return ::multithreadingMinimax(state, state.getActivePlayerId(), depth).move;
    }

}