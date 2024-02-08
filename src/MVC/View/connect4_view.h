// connect4_view.h
// by Jake Charles Osborne III
#pragma once



#include "../Model/connect4_model.h"
#include <string>



namespace connect4::view {

	void printHeader();
	void printMessage(const std::string&);
	void printCurrentTurn(const connect4::model::Board&);
	void printBoard(const connect4::model::Board&);
	void printHelpMenu();
	std::string promptUser();

}