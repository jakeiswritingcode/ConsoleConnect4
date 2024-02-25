// qlstate.h
// by Jake Chareles Osborne III
#pragma once



# include "../state_action_interface.h"
#include <vector>
#include <memory>



namespace ai {

	class QLState : public virtual State {
	public:
		std::shared_ptr<QLState> cloneQLState() const {
			return std::dynamic_pointer_cast<QLState>(clone());
		}

		virtual std::vector<float> extractFeatures() const = 0;

		virtual ~QLState() {}
	};

	class SharedQLState : public virtual State {
	public:
		std::shared_ptr<SharedQLState> cloneSharedQLState() const {
			return std::dynamic_pointer_cast<SharedQLState>(clone());
		}

		virtual short getActiveQLAgentId() const = 0;
		virtual std::vector<float> extractFeatures(short agentId) const = 0;

		virtual ~SharedQLState() {}
	};

}