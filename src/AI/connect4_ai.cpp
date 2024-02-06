// connect4_ai.cpp
// by Jake Charles Osborne III



#include "connect4_ai.h"
//#include "../MVC/Model/connect4_model.h"
#include <vector>
#include <future>
#include <optional>

using std::vector;
using std::future;
using std::async;
using std::optional;
using std::nullopt;



namespace {

    struct MinimaxResult {
        int moveIndex;
        double moveScore;
    };

    double evaluateBoard(const connect4::model::Board& board, const short& aiPlayer) {
        short winner = board.checkForWin();
        switch (winner) {
        case 1:
            return aiPlayer == 1 ? DBL_MAX : DBL_MIN;
        case 2:
            return aiPlayer == 2 ? DBL_MAX : DBL_MIN;
        case -1:
            return 0; // Tie
        }

        // TODO: evaluate board
        return 4;
    }

    MinimaxResult maximize(connect4::model::Board board, const short& aiPlayer, int depth) {
        MinimaxResult bestResult = { -1, DBL_MIN };

        for (int i = 0; i < 7; i++) {
            if (board.colAvailable(i)) {
                connect4::model::Board nextBoard = board;
                nextBoard.addPiece(i);

                double score = minimax(board, aiPlayer, depth - 1).moveScore;

                if (score > bestResult.moveScore) {
                    bestResult = { i, score };
                }
            }
        }

        return bestResult;
    }

    MinimaxResult multithreadingMaximize(connect4::model::Board board, const short& aiPlayer, int depth) {
        MinimaxResult bestResult = { -1, DBL_MIN };

        optional<future<MinimaxResult>> optFutures[7];
        for (int i = 0; i < 7; i++) {
            if (board.colAvailable(i)) {
                connect4::model::Board nextBoard = board;
                nextBoard.addPiece(i);
                optFutures[i] = async(std::launch::async,
                    minimax, board, aiPlayer, depth - 1);
            }
            else {
                optFutures[i] = nullopt;
            }
        }
        for (int i = 0; i < 7; i) {
            if (optFutures[i]) {
                MinimaxResult minimaxResult = optFutures[i]->get();
                if (minimaxResult.moveScore > bestResult.moveScore) {
                    bestResult = { i, minimaxResult.moveScore };
                }
            }
        }

        return bestResult;
    }

    MinimaxResult minimize(connect4::model::Board board, const short& aiPlayer, int depth) {
        MinimaxResult bestResult = { -1, DBL_MAX };

        for (int i = 0; i < 7; i++) {
            if (board.colAvailable(i)) {
                connect4::model::Board nextBoard = board;
                nextBoard.addPiece(i);

                double score = minimax(board, aiPlayer, depth - 1).moveScore;

                if (score < bestResult.moveScore) {
                    bestResult = { i, score };
                }
            }
        }

        return bestResult;
    }

    MinimaxResult multithreadingMinimize(connect4::model::Board board, const short& aiPlayer, int depth) {
        MinimaxResult bestResult = { -1, DBL_MAX };

        optional<future<MinimaxResult>> optFutures[7];
        for (int i = 0; i < 7; i++) {
            if (board.colAvailable(i)) {
                connect4::model::Board nextBoard = board;
                nextBoard.addPiece(i);
                optFutures[i] = async(std::launch::async,
                    minimax, board, aiPlayer, depth - 1);
            }
            else {
                optFutures[i] = nullopt;
            }
        }
        for (int i = 0; i < 7; i) {
            if (optFutures[i]) {
                MinimaxResult minimaxResult = optFutures[i]->get();
                if (minimaxResult.moveScore < bestResult.moveScore) {
                    bestResult = { i, minimaxResult.moveScore };
                }
            }
        }

        return bestResult;
    }

    MinimaxResult minimax(connect4::model::Board board, const short& aiPlayer, int depth) {
        if (depth == 0) return { -1, evaluateBoard(board, aiPlayer) };

        MinimaxResult bestResult;
        if (aiPlayer == board.getActivePlayer()) {
            return maximize(board, aiPlayer, depth);
        }
        else {
            return minimize(board, aiPlayer, depth);
        }
    }

}

namespace connect4::ai {

    short generateMove(const model::Board& board, const short& aiPlayer) {
        return minimax(board, aiPlayer, 5).moveIndex;
    }

}