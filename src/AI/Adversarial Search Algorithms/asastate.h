// asastate.h
// by Jake Chareles Osborne III
#pragma once



#include "../state_action_interface.h"
#include <vector>
#include <memory>



namespace ai {

	class ASAState : public virtual State {
	public:
		std::shared_ptr<ASAState> cloneASAState() const {
			return std::dynamic_pointer_cast<ASAState>(clone());
		}

		virtual short getActiveASAAgentId() const = 0;
		virtual double evaluate(short maximizingAgentId) const = 0;

		virtual ~ASAState() {}
	};

}