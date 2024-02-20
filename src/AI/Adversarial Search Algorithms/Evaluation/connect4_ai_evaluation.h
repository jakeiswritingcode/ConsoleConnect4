// connect4_ai_evaluation.h
// by Jake Charles Osborne III
#pragma once



#include "../../../MVC/Model/connect4_model.h"



namespace connect4::ai {

	double evaluate(const connect4::model::Board&, short maximizingPlayer);

}