// connect4_ai_alphabeta.cpp
// by Jake Charles Osborne III



#include "connect4_ai_adversarial_search_algorithms.h"
#include "Evaluation/connect4_ai_evaluation.h"
#include "../../MVC/Model/connect4_model.h"
#include <mutex>
#include <future>
#include <optional>

using namespace connect4;
using std::mutex;
using std::future;
using std::async;
using std::optional;
using std::nullopt;



namespace {

    ai::MinimaxResult alphaBeta(const model::Board& board, short maximizingPlayer, int depth, double& alpha, double& beta);

    ai::MinimaxResult minimize(model::Board board, short maximizingPlayer, int depth, double alpha, double beta) {
        ai::MinimaxResult bestResult = { -1, DBL_MAX };

        for (short i = 0; i < 7; ++i) {
            if (board.colAvailable(i)) {
                model::Board nextBoard = board;
                nextBoard.addPiece(i);

                double score = alphaBeta(nextBoard, maximizingPlayer, depth - 1, alpha, beta).moveScore;
                if (score < bestResult.moveScore) bestResult = { i, score };

                if (beta > score) beta = score;
                if (beta <= alpha) break;
            }
        }

        return bestResult;
    }

    ai::MinimaxResult maximize(model::Board board, short maximizingPlayer, int depth, double& alpha, double& beta) {
        ai::MinimaxResult bestResult = { -1, -DBL_MAX };

        for (short i = 0; i < 7; ++i) {
            if (board.colAvailable(i)) {
                model::Board nextBoard = board;
                nextBoard.addPiece(i);

                double score = alphaBeta(nextBoard, maximizingPlayer, depth - 1, alpha, beta).moveScore;
                if (score > bestResult.moveScore) bestResult = { i, score };

                if (alpha < score) alpha = score;
                if (alpha >= beta) break;
            }
        }

        return bestResult;
    }

    ai::MinimaxResult alphaBeta(const model::Board& board, short maximizingPlayer, int depth, double& alpha, double& beta) {
        short winner = board.checkForWin();
        if (winner == maximizingPlayer) return { -1, DBL_MAX };
        if (winner == 1 || winner == 2) return { -1, -DBL_MAX };
        if (winner == -1) return { -1, 0 }; // tie
        if (depth <= 0) return { -1, ai::evaluate(board, maximizingPlayer) };

        if (maximizingPlayer == board.getActivePlayer()) {
            return maximize(board, maximizingPlayer, depth, alpha, beta);
        }
        else {
            return minimize(board, maximizingPlayer, depth, alpha, beta);
        }
    }

}

namespace {

    struct PruningValues {
        double alpha = -DBL_MAX;
        double beta = DBL_MAX;
        mutex mtx;
    };

    ai::MinimaxResult multithreadingAlphaBeta(const model::Board& board, short maximizingPlayer, int depth, PruningValues& pVals);

    ai::MinimaxResult multithreadingMinimize(const model::Board& board, short maximizingPlayer, int depth, PruningValues& pVals) {
        ai::MinimaxResult bestResult = { -1, DBL_MAX };

        optional<future<ai::MinimaxResult>> optFutures[7];
        for (short i = 0; i < 7; ++i) {
            if (board.colAvailable(i)) {
                model::Board nextBoard = board;
                nextBoard.addPiece(i);
                optFutures[i] = async(std::launch::async,
                    multithreadingAlphaBeta, nextBoard, maximizingPlayer, depth - 1, std::ref(pVals));
            }
            else {
                optFutures[i] = nullopt;
            }
        }
        for (short i = 0; i < 7; ++i) {
            if (optFutures[i]) {
                double score = optFutures[i]->get().moveScore;
                if (score < bestResult.moveScore) bestResult = { i, score };

                pVals.mtx.lock();
                if (pVals.beta > score) pVals.beta = score;
                if (pVals.beta <= pVals.alpha) i = 7;
                pVals.mtx.unlock();
            }
        }

        return bestResult;
    }

    ai::MinimaxResult multithreadingMaximize(const model::Board& board, short maximizingPlayer, int depth, PruningValues& pVals) {
        ai::MinimaxResult bestResult = { -1, -DBL_MAX };

        optional<future<ai::MinimaxResult>> optFutures[7];
        for (short i = 0; i < 7; ++i) {
            if (board.colAvailable(i)) {
                model::Board nextBoard = board;
                nextBoard.addPiece(i);

                optFutures[i] = async(std::launch::async,
                    multithreadingAlphaBeta, nextBoard, maximizingPlayer, depth - 1, std::ref(pVals));
            }
            else {
                optFutures[i] = nullopt;
            }
        }
        for (short i = 0; i < 7; ++i) {
            if (optFutures[i]) {
                double score = optFutures[i]->get().moveScore;
                if (score > bestResult.moveScore) bestResult = { i, score };

                pVals.mtx.lock();
                if (pVals.beta > score) pVals.beta = score;
                if (pVals.beta <= pVals.alpha) i = 7;
                pVals.mtx.unlock();
            }
        }

        return bestResult;
    }

    ai::MinimaxResult multithreadingAlphaBeta(const model::Board& board, short maximizingPlayer, int depth, PruningValues& pVals) {
        short winner = board.checkForWin();
        if (winner == maximizingPlayer) return { -1, DBL_MAX };
        if (winner == 1 || winner == 2) return { -1, -DBL_MAX };
        if (winner == -1) return { -1, 0 }; // tie
        if (depth <= 0) return { -1, ai::evaluate(board, maximizingPlayer) };

        if (maximizingPlayer == board.getActivePlayer()) {
            return multithreadingMaximize(board, maximizingPlayer, depth, pVals);
        }
        else {
            return multithreadingMinimize(board, maximizingPlayer, depth, pVals);
        }
    }

}

namespace connect4::ai {

    MinimaxResult alphaBeta(const model::Board& board, short maximizingPlayer, int depth) {
        double alpha = -DBL_MAX;
        double beta = DBL_MAX;
        return ::alphaBeta(board, maximizingPlayer, depth, alpha, beta);
    }

    MinimaxResult multithreadingAlphaBeta(const connect4::model::Board& board, short maximizingPlayer, int depth) {
        PruningValues pVals;
        return ::multithreadingAlphaBeta(board, maximizingPlayer, depth, pVals);
    }

}