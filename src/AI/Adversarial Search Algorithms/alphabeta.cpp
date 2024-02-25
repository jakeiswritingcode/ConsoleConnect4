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
        double score;
    };

}

namespace {

    AlphaBetaResult alphaBeta(const ai::ASAState& state, short maximizingAgentId, int depth, double& alpha, double& beta);

    AlphaBetaResult minimize(const ai::ASAState& state, short maximizingAgentId, int depth, double alpha, double beta) {
        AlphaBetaResult bestResult = { nullptr, DBL_MAX };

        for (auto& action : state.getActions()) {
            auto nextState = state.cloneASAState();
            nextState->takeAction(action);

            double score = ::alphaBeta(*nextState, maximizingAgentId, depth - 1, alpha, beta).score;
            if (score < bestResult.score) bestResult = { action, score };

            if (beta > score) beta = score;
            if (beta <= alpha) break;
        }

        return bestResult;
    }

    AlphaBetaResult maximize(const ai::ASAState& state, short maximizingAgentId, int depth, double& alpha, double& beta) {
        AlphaBetaResult bestResult = { nullptr, -DBL_MAX };

        for (auto& action : state.getActions()) {
            auto nextState = state.cloneASAState();
            nextState->takeAction(action);

            double score = ::alphaBeta(*nextState, maximizingAgentId, depth - 1, alpha, beta).score;
            if (score > bestResult.score) bestResult = { action, score };

            if (alpha < score) alpha = score;
            if (alpha >= beta) break;
        }

        return bestResult;
    }

    AlphaBetaResult alphaBeta(const ai::ASAState& state, short maximizingAgentId, int depth, double& alpha, double& beta) {
        if (state.isTerminal() || depth <= 0) return { nullptr, state.evaluate(maximizingAgentId) };

        if (maximizingAgentId == state.getActiveASAAgentId()) {
            return maximize(state, maximizingAgentId, depth, alpha, beta);
        }
        else {
            return minimize(state, maximizingAgentId, depth, alpha, beta);
        }
    }

}

namespace {

    struct PruningValues {
        double alpha = -DBL_MAX;
        double beta = DBL_MAX;
        mutex mtx;
    };

    AlphaBetaResult multithreadingAlphaBeta(shared_ptr<ai::ASAState> state, short maximizingAgentId, int depth, PruningValues& pVals);

    AlphaBetaResult multithreadingMinimize(shared_ptr<ai::ASAState> state, short maximizingAgentId, int depth, PruningValues& pVals) {
        AlphaBetaResult bestResult = { nullptr, DBL_MAX };

        auto actions = state->getActions();
        vector<future<AlphaBetaResult>> futures;
        futures.reserve(actions.size());
        for (int i = 0; i < actions.size(); ++i) {
            auto nextState = state->cloneASAState();
            nextState->takeAction(actions[i]);
            futures.emplace_back(async(std::launch::async,
                multithreadingAlphaBeta, nextState, maximizingAgentId, depth - 1, std::ref(pVals)));
        }
        for (int i = 0; i < futures.size(); ++i) {
            double score = futures[i].get().score;
            if (score < bestResult.score) bestResult = { actions[i], score };

            pVals.mtx.lock();
            if (pVals.beta > score) pVals.beta = score;
            if (pVals.beta <= pVals.alpha) i = 7;
            pVals.mtx.unlock();
        }

        return bestResult;
    }

    AlphaBetaResult multithreadingMaximize(shared_ptr<ai::ASAState> state, short maximizingAgentId, int depth, PruningValues& pVals) {
        AlphaBetaResult bestResult = { nullptr, -DBL_MAX };
        
        auto actions = state->getActions();
        vector<future<AlphaBetaResult>> futures;
        futures.reserve(actions.size());
        for (int i = 0; i < actions.size(); ++i) {
            auto nextState = state->cloneASAState();
            nextState->takeAction(actions[i]);
            futures.emplace_back(async(std::launch::async,
                multithreadingAlphaBeta, nextState, maximizingAgentId, depth - 1, std::ref(pVals)));
        }
        for (int i = 0; i < futures.size(); ++i) {
            double score = futures[i].get().score;
            if (score > bestResult.score) bestResult = { actions[i], score };

            pVals.mtx.lock();
            if (pVals.beta > score) pVals.beta = score;
            if (pVals.beta <= pVals.alpha) i = 7;
            pVals.mtx.unlock();
        }

        return bestResult;
    }

    AlphaBetaResult multithreadingAlphaBeta(shared_ptr<ai::ASAState> state, short maximizingAgentId, int depth, PruningValues& pVals) {
        if (state->isTerminal() || depth <= 0) return { nullptr, state->evaluate(maximizingAgentId) };

        if (maximizingAgentId == state->getActiveASAAgentId()) {
            return multithreadingMaximize(state, maximizingAgentId, depth, pVals);
        }
        else {
            return multithreadingMinimize(state, maximizingAgentId, depth, pVals);
        }
    }

}

namespace ai {

    shared_ptr<Action> alphaBeta(const ASAState& state, int depth) {
        double alpha = -DBL_MAX;
        double beta = DBL_MAX;
        return ::alphaBeta(state, state.getActiveASAAgentId(), depth, alpha, beta).move;
    }

    shared_ptr<Action> multithreadingAlphaBeta(const ASAState& state, int depth) {
        PruningValues pVals;
        return ::multithreadingAlphaBeta(state.cloneASAState(), state.getActiveASAAgentId(), depth, pVals).move;
    }

}