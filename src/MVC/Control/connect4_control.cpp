// connect4_control.cpp
// by Jake Charles Osborne III



#include "connect4_control.h"
#include "../Model/connect4_model.h"
#include "../View/connect4_view.h"
#include "../../AI/connect4_ai.h"
#include <iostream>
#include <string>

using namespace connect4;
using std::string;



namespace {

	const string HELP_MESSAGE = "Enter \"help\" for a list of commands.";

	enum UserAction {
		makeMove,
		reset,
		aiPlayer1,
		aiPlayer2,
		help,
		exitGame,
		invalidAction
	};

	UserAction parseInput(string& input,  connect4::model::Board& board) {
		for (auto& inputChar : input) inputChar = tolower(inputChar);

		if (!input.empty() && std::isdigit(input[0])) return UserAction::makeMove;
		if (input == "r" || input == "reset") return UserAction::reset;
		if (input == "ai-player1") return UserAction::aiPlayer1;
		if (input == "ai-player2") return UserAction::aiPlayer2;
		if (input == "h" || input == "help") return UserAction::help;
		if (input == "e" || input == "exit") return UserAction::exitGame;

		return UserAction::invalidAction;
	}

}

namespace connect4 {

	void play() {
		string input;
		UserAction userAction = invalidAction;

		short ai = 0;
		
		while (std::cin && userAction != UserAction::exitGame) {

			model::Board board;
			short winner = 0;
			string message = HELP_MESSAGE;
			while (std::cin && winner == 0) {
				if (ai != board.getActivePlayer()) {
					view::printHeader();
					view::printMessage(message);
					view::printCurrentTurn(board);
					view::printBoard(board);
					input = view::promptUser();

					userAction = parseInput(input, board);
					switch (userAction) {
					case UserAction::reset:
						board.reset();
						message = "Game reset.";
						ai = 0;
						break;

					case UserAction::aiPlayer1:
						board.reset();
						message = "Game reset with AI enabled as Player1.";
						ai = 1;
						break;

					case UserAction::aiPlayer2:
						board.reset();
						message = "Game reset with AI enabled as Player2.";
						ai = 2;
						break;

					case UserAction::help:
						view::printHelpMenu();
						break;

					case UserAction::makeMove:
						if (input.size() == 1 &&
							input[0] >= '1' && input[0] <= '7' &&
							board.getBoardGrid()[5][input[0] - '1'] == 0)
						{
							board.addPiece(input[0] - '1');
							if (board.turnCount % 2 == 1) {
								message = "Player1 move complete.";
							}
							else {
								message = "Player2 move complete.";
							}
						}
						else {
							message = "Invalid move detected. " + HELP_MESSAGE;
						}
						break;

					case UserAction::invalidAction:
						message = "Invalid move detected. " + HELP_MESSAGE;
						break;
					}
				}
				else {
					short move = ai::generateMove(board);
					board.addPiece(move);
					message = "AI move complete.";
				}

				winner = board.checkForWin();
			}

			if (winner == 1) {
				message = "Player1 won! Enter any value to continue.";
			}
			else if (winner == 2) {
				message = "Player2 won! Enter any value to continue.";
			}
			else {
				message = "Tie! Enter any value to continue.";
			}

			view::printHeader();
			view::printMessage(message);
			view::printCurrentTurn(board);
			view::printBoard(board);
			input = view::promptUser();
		}
	}

}