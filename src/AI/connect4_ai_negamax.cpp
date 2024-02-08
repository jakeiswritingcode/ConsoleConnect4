// connect4_ai_negamax.h
// by Jake Charles Osborne III



#include "connect4_ai_searchtree_algorithms.h"
#include "Evaluation/connect4_ai_evaluation.h"
#include "../MVC/Model/connect4_model.h"
#include <future>
#include <optional>

using namespace connect4;
using std::future;
using std::async;
using std::optional;
using std::nullopt;



namespace connect4::ai {

    MinimaxResult negamax(const model::Board& board, short maximizingPlayer, int depth) {
        short winner = board.checkForWin();
        if (winner == maximizingPlayer) return { -1, DBL_MAX };
        if (winner == 1 || winner == 2) return { -1, -DBL_MAX };
        if (winner == -1) return { -1, 0 }; // tie
        if (depth <= 0) return { -1, evaluate(board, maximizingPlayer) };

        ai::MinimaxResult bestResult = { -1, -DBL_MAX };

        for (short i = 0; i < 7; ++i) {
            if (board.colAvailable(i)) {
                model::Board nextBoard = board;
                nextBoard.addPiece(i);

                double score = -negamax(nextBoard, maximizingPlayer, depth - 1).moveScore;
                if (score > bestResult.moveScore) bestResult = { i, score };
            }
        }

        return bestResult;
    }

    MinimaxResult multithreadingNegamax(const model::Board& board, short maximizingPlayer, int depth) {
        short winner = board.checkForWin();
        if (winner == maximizingPlayer) return { -1, DBL_MAX };
        if (winner == 1 || winner == 2) return { -1, -DBL_MAX };
        if (winner == -1) return { -1, 0 }; // tie
        if (depth <= 0) return { -1, evaluate(board, maximizingPlayer) };

        ai::MinimaxResult bestResult = { -1, -DBL_MAX };

        optional<future<ai::MinimaxResult>> optFutures[7];
        for (short i = 0; i < 7; ++i) {
            if (board.colAvailable(i)) {
                model::Board nextBoard = board;
                nextBoard.addPiece(i);
                optFutures[i] = async(std::launch::async,
                    ai::multithreadingNegamax, nextBoard, maximizingPlayer, depth - 1);
            }
            else {
                optFutures[i] = nullopt;
            }
        }
        for (short i = 0; i < 7; ++i) {
            if (optFutures[i]) {
                double score = optFutures[i]->get().moveScore;
                if (score > bestResult.moveScore) bestResult = { i, score };
            }
        }

        return bestResult;
    }

}