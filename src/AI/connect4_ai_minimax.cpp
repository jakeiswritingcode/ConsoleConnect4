// connect4_ai_minimax.cpp
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



namespace {

    ai::MinimaxResult minimize(const model::Board& board, short maximizingPlayer, int depth) {
        ai::MinimaxResult bestResult = { -1, DBL_MAX };

        for (short i = 0; i < 7; ++i) {
            if (board.colAvailable(i)) {
                model::Board nextBoard = board;
                nextBoard.addPiece(i);

                double score = ai::minimax(nextBoard, maximizingPlayer, depth - 1).moveScore;
                if (score < bestResult.moveScore) bestResult = { i, score };
            }
        }

        return bestResult;
    }

    ai::MinimaxResult maximize(const model::Board& board, short maximizingPlayer, int depth) {
        ai::MinimaxResult bestResult = { -1, -DBL_MAX };

        for (short i = 0; i < 7; ++i) {
            if (board.colAvailable(i)) {
                model::Board nextBoard = board;
                nextBoard.addPiece(i);

                double score = ai::minimax(nextBoard, maximizingPlayer, depth - 1).moveScore;
                if (score > bestResult.moveScore) bestResult = { i, score };
            }
        }

        return bestResult;
    }

    ai::MinimaxResult multithreadingMinimize(const model::Board& board, short maximizingPlayer, int depth) {
        ai::MinimaxResult bestResult = { -1, DBL_MAX };

        optional<future<ai::MinimaxResult>> optFutures[7];
        for (short i = 0; i < 7; ++i) {
            if (board.colAvailable(i)) {
                model::Board nextBoard = board;
                nextBoard.addPiece(i);
                optFutures[i] = async(std::launch::async,
                    ai::multithreadingMinimax, nextBoard, maximizingPlayer, depth - 1);
            }
            else {
                optFutures[i] = nullopt;
            }
        }
        for (short i = 0; i < 7; ++i) {
            if (optFutures[i]) {
                double score = optFutures[i]->get().moveScore;
                if (score < bestResult.moveScore) bestResult = { i, score };
            }
        }

        return bestResult;
    }

    ai::MinimaxResult multithreadingMaximize(const model::Board& board, short maximizingPlayer, int depth) {
        ai::MinimaxResult bestResult = { -1, -DBL_MAX };

        optional<future<ai::MinimaxResult>> optFutures[7];
        for (short i = 0; i < 7; ++i) {
            if (board.colAvailable(i)) {
                model::Board nextBoard = board;
                nextBoard.addPiece(i);
                optFutures[i] = async(std::launch::async,
                    ai::multithreadingMinimax, nextBoard, maximizingPlayer, depth - 1);
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

namespace connect4::ai {

    MinimaxResult minimax(const model::Board& board, short maximizingPlayer, int depth) {
        short winner = board.checkForWin();
        if (winner == maximizingPlayer) return { -1, DBL_MAX };
        if (winner == 1 || winner == 2) return { -1, -DBL_MAX };
        if (winner == -1) return { -1, 0 }; // tie
        if (depth <= 0) return { -1, evaluate(board, maximizingPlayer) };

        if (maximizingPlayer == board.getActivePlayer()) {
            return maximize(board, maximizingPlayer, depth);
        }
        else {
            return minimize(board, maximizingPlayer, depth);
        }
    }

    MinimaxResult multithreadingMinimax(const model::Board& board, short maximizingPlayer, int depth) {
        short winner = board.checkForWin();
        if (winner == maximizingPlayer) return { -1, DBL_MAX };
        if (winner == 1 || winner == 2) return { -1, -DBL_MAX };
        if (winner == -1) return { -1, 0 }; // tie
        if (depth <= 0) return { -1, evaluate(board, maximizingPlayer) };

        if (maximizingPlayer == board.getActivePlayer()) {
            return multithreadingMaximize(board, maximizingPlayer, depth);
        }
        else {
            return multithreadingMinimize(board, maximizingPlayer, depth);
        }
    }

}