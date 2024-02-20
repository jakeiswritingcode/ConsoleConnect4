#include <vector>
#include <memory>

namespace connect4::ai {

	struct Action {
		// pure virtual destructor to allow dynamic casting
		virtual ~Action() = 0 {};
	};

	class State {
	public:
		virtual std::shared_ptr<State> clone() const = 0;
		virtual std::vector<float> extractFeatures() const = 0;
		virtual std::vector<std::shared_ptr<Action>> getActions() const = 0;
		virtual void useAction(std::shared_ptr<Action>) = 0;
		virtual bool isTerminal() const = 0;

		virtual ~State() {}
	};

}