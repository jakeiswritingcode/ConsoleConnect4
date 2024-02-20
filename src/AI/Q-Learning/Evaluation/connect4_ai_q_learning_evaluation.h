// connect4_ai_q_learning_evaluation.h
// by Jake Charles Osborne III
#pragma once



#include "../../../MVC/Model/connect4_model.h"
#include <vector>



namespace connect4::ai {

	std::vector<float> extractFeatures(const connect4::model::Board& board);

}