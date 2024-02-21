// asastate.h
// by Jake Chareles Osborne III
#pragma once



# include "../state_action_interface.h"
#include <vector>
#include <memory>



namespace ai {

	class ASAState : virtual public State {
	public:
		std::shared_ptr<ASAState> cloneASAState() const {
			return std::dynamic_pointer_cast<ASAState>(clone());
		}

		virtual short getActivePlayerId() const = 0;

		virtual double evaluate(short maximizingPlayerId) const = 0;

		virtual ~ASAState() {}
	};

}