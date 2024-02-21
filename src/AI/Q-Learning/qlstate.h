// qlstate.h
// by Jake Chareles Osborne III
#pragma once



# include "../state_action_interface.h"
#include <vector>
#include <memory>



namespace ai {

	class QLState : virtual public State {
	public:
		std::shared_ptr<QLState> cloneQLState() const {
			return std::dynamic_pointer_cast<QLState>(clone());
		}

		virtual std::vector<float> extractFeatures() const = 0;

		virtual ~QLState() {}
	};

}