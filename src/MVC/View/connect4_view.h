// connect4_view.h
// by Jake Charles Osborne III
#pragma once



#include <bitset>
#include <array>

using std::bitset;
using std::array;



namespace connect4::view {

	void printHeader();
	void printMessage(const std::string&);
	void printCurrentTurn(const connect4::model::Board&);
	void printBoard(const connect4::model::Board&);
	void printHelpMenu();
	std::string promptUser();

}