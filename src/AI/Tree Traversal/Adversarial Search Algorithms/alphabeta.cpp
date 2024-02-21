// alphabeta.cpp
// by Jake Charles Osborne III



#include "adversarial_search_algorithms.h"
#include <memory>
#include <vector>
#include <mutex>
#include <future>

using std::shared_ptr;
using std::vector;
using std::mutex;
using std::future;
using std::async;



namespace {

    struct AlphaBetaResult {
        shared_ptr<ai::Action> move;
        double moveScore;
    };

}

namespace {

    AlphaBetaResult alphaBeta(const ai::ASAState& state, short maximizingPlayerId, int depth, double& alpha, double& beta);

    AlphaBetaResult minimize(const ai::ASAState& state, short maximizingPlayerId, int depth, double alpha, double beta) {
        AlphaBetaResult bestResult = { nullptr, DBL_MAX };

        for (auto& action : state.getActions()) {
            auto nextState = state.cloneASAState();
            nextState->useAction(action);

            double score = ::alphaBeta(*nextState, maximizingPlayerId, depth - 1, alpha, beta).moveScore;
            if (score < bestResult.moveScore) bestResult = { action, score };

            if (beta > score) beta = score;
            if (beta <= alpha) break;
        }

        return bestResult;
    }

    AlphaBetaResult maximize(const ai::ASAState& state, short maximizingPlayerId, int depth, double& alpha, double& beta) {
        AlphaBetaResult bestResult = { nullptr, -DBL_MAX };

        for (auto& action : state.getActions()) {
            auto nextState = state.cloneASAState();
            nextState->useAction(action);

            double score = ::alphaBeta(*nextState, maximizingPlayerId, depth - 1, alpha, beta).moveScore;
            if (score > bestResult.moveScore) bestResult = { action, score };

            if (alpha < score) alpha = score;
            if (alpha >= beta) break;
        }

        return bestResult;
    }

    AlphaBetaResult alphaBeta(const ai::ASAState& state, short maximizingPlayerId, int depth, double& alpha, double& beta) {
        if (state.isTerminal() || depth <= 0) return { nullptr, state.evaluate(maximizingPlayerId) };

        if (maximizingPlayerId == state.getActivePlayerId()) {
            return maximize(state, maximizingPlayerId, depth, alpha, beta);
        }
        else {
            return minimize(state, maximizingPlayerId, depth, alpha, beta);
        }
    }

}

namespace {

    struct PruningValues {
        double alpha = -DBL_MAX;
        double beta = DBL_MAX;
        mutex mtx;
    };

    AlphaBetaResult multithreadingAlphaBeta(const ai::ASAState& state, short maximizingPlayerId, int depth, PruningValues& pVals);

    AlphaBetaResult multithreadingMinimize(const ai::ASAState& state, short maximizingPlayerId, int depth, PruningValues& pVals) {
        AlphaBetaResult bestResult = { nullptr, DBL_MAX };

        auto actions = state.getActions();
        vector<future<AlphaBetaResult>> futures;
        futures.reserve(actions.size());
        for (int i = 0; i < actions.size(); ++i) {
            auto nextState = state.cloneASAState();
            nextState->useAction(actions[i]);
            futures.emplace_back(async(std::launch::async,
                multithreadingAlphaBeta, *nextState, maximizingPlayerId, depth - 1, std::ref(pVals)));
        }
        for (int i = 0; i < futures.size(); ++i) {
            double score = futures[i].get().moveScore;
            if (score < bestResult.moveScore) bestResult = { actions[i], score };

            pVals.mtx.lock();
            if (pVals.beta > score) pVals.beta = score;
            if (pVals.beta <= pVals.alpha) i = 7;
            pVals.mtx.unlock();
        }

        return bestResult;
    }

    AlphaBetaResult multithreadingMaximize(const ai::ASAState& state, short maximizingPlayerId, int depth, PruningValues& pVals) {
        AlphaBetaResult bestResult = { nullptr, -DBL_MAX };
        
        auto actions = state.getActions();
        vector<future<AlphaBetaResult>> futures;
        futures.reserve(actions.size());
        for (int i = 0; i < actions.size(); ++i) {
            auto nextState = state.cloneASAState();
            nextState->useAction(actions[i]);
            futures.emplace_back(async(std::launch::async,
                multithreadingAlphaBeta, *nextState, maximizingPlayerId, depth - 1, std::ref(pVals)));
        }
        for (int i = 0; i < futures.size(); ++i) {
            double score = futures[i].get().moveScore;
            if (score > bestResult.moveScore) bestResult = { actions[i], score };

            pVals.mtx.lock();
            if (pVals.beta > score) pVals.beta = score;
            if (pVals.beta <= pVals.alpha) i = 7;
            pVals.mtx.unlock();
        }

        return bestResult;
    }

    AlphaBetaResult multithreadingAlphaBeta(const ai::ASAState& state, short maximizingPlayerId, int depth, PruningValues& pVals) {
        if (state.isTerminal() || depth <= 0) return { nullptr, state.evaluate(maximizingPlayerId) };

        if (maximizingPlayerId == state.getActivePlayerId()) {
            return multithreadingMaximize(state, maximizingPlayerId, depth, pVals);
        }
        else {
            return multithreadingMinimize(state, maximizingPlayerId, depth, pVals);
        }
    }

}

namespace ai {

    shared_ptr<Action> alphaBeta(const ai::ASAState& state, int depth) {
        double alpha = -DBL_MAX;
        double beta = DBL_MAX;
        return ::alphaBeta(state, state.getActivePlayerId(), depth, alpha, beta).move;
    }

    shared_ptr<Action> multithreadingAlphaBeta(const ai::ASAState& state, int depth) {
        PruningValues pVals;
        return ::multithreadingAlphaBeta(state, state.getActivePlayerId(), depth, pVals).move;
    }

}