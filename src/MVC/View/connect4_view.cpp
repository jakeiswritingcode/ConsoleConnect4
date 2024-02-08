// connect4_view.cpp
// by Jake Charles Osborne III



#include "connect4_view.h"
#include "../Model/connect4_model.h"
#include <iostream>
#include <string>
#include <array>

using std::cin;
using std::cout;
using std::getline;
using std::string;
using std::array;



namespace {

	const char* WINDOW_MARGIN = "     ";
	const char* PROMPT = "->";

}


namespace connect4::view {
	
	void printHeader() {
		cout << "\n\n\n"
			<< "Connect4 with AI" << '\n'
			<< "more at jco-iii.com" << '\n'
			<< '\n';
	}

	void printMessage(const string& message) {
		cout << message << "\n\n";
	}

	void printCurrentTurn(const connect4::model::Board& board) {
		if (board.turnCount % 2 == 0) {
			cout << "Player 1's turn (O)" << '\n';
		}
		else {
			cout << "Player 2's turn (X)" << '\n';
		}
		cout << '\n';
	}

	void printBoard(const connect4::model::Board& board) {
		array<array<short, 7>, 6> boardGrid = board.getBoardGrid();
		for (int row = 5; row >= 0; --row) {
			cout << WINDOW_MARGIN << '|';
			for (int col = 0; col < 7; ++col) {
				switch (boardGrid[row][col]) {
				case 0:
					cout << ' ';
					break;
				case 1:
					cout << 'O';
					break;
				case 2:
					cout << 'X';
					break;
				}
			}
			cout << '|' << '\n';
		}
		cout << WINDOW_MARGIN << " 1234567 " << "\n\n";
	}

	void printMessage(char* message) {
		cout << WINDOW_MARGIN << message << "\n\n";
	}

	void printHelpMenu() {
		cout << "\n\n\n"
			<< WINDOW_MARGIN << "Commands:\n"
			<< "\n"
			<< "\n"
			<< WINDOW_MARGIN << "\"h\" or \"help\"\n"
			<< WINDOW_MARGIN << "display help screen\n"\
			<< "\n"
			<< WINDOW_MARGIN << "\"ai-player1\"\n"
			<< WINDOW_MARGIN << "start new game with AI playing first\n"
			<< "\n"
			<< WINDOW_MARGIN << "\"ai-player2\"\n"
			<< WINDOW_MARGIN << "start new game with AI playing second\n"
			<< "\n"
			<< WINDOW_MARGIN << "\"reset\"\n"
			<< WINDOW_MARGIN << "begin a new game without AI\n"
			<< "\n"
			<< WINDOW_MARGIN << "\"exit\"\n"
			<< WINDOW_MARGIN << "exit the application\n"
			<< "\n"
			<< "\n"
			<< WINDOW_MARGIN << "Press enter to resume the game.\n"
			<< PROMPT;

		string throwaway;
		getline(cin, throwaway);
	}

	string promptUser() {
		cout << "      " << PROMPT;

		string input;
		getline(cin, input);
		return input;
	}

}