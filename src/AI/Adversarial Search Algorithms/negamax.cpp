// negamax.h
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

    struct NegamaxResult {
        shared_ptr<ai::Action> move;
        double score;
    };

}

namespace {

    NegamaxResult negamax(const ai::ASAState& state, int depth) {
        if (state.isTerminal() || depth <= 0) return { nullptr, state.evaluate(state.getActiveASAAgentId()) };

        NegamaxResult bestResult = { nullptr, -DBL_MAX };

        for (auto& action : state.getActions()) {
            auto nextState = state.cloneASAState();
            nextState->takeAction(action);

            double score = -::negamax(*nextState, depth - 1).score;
            if (score > bestResult.score) bestResult = { action, score };
        }

        return bestResult;
    }

}

namespace {

    NegamaxResult multithreadingNegamax(shared_ptr<ai::ASAState> state, int depth) {
        if (state->isTerminal() || depth <= 0) return { nullptr, state->evaluate(state->getActiveASAAgentId()) };

        NegamaxResult bestResult = { nullptr, -DBL_MAX };

        auto actions = state->getActions();
        vector<future<NegamaxResult>> futures;
        futures.reserve(actions.size());
        for (int i = 0; i < actions.size(); ++i) {
            auto nextState = state->cloneASAState();
            nextState->takeAction(actions[i]);
            futures.emplace_back(async(std::launch::async,
                ::multithreadingNegamax, nextState, depth - 1));
        }
        for (short i = 0; i < futures.size(); ++i) {
            double score = -futures[i].get().score;
            if (score > bestResult.score) bestResult = { actions[i], score };
        }

        return bestResult;
    }

}

namespace ai {

    shared_ptr<Action> negamax(const ai::ASAState& state, int depth) {
        return ::negamax(state, depth).move;
    }

    shared_ptr<Action> multithreadingNegamax(const ai::ASAState& state, int depth) {
        return ::multithreadingNegamax(state.cloneASAState(), depth).move;
    }

}

