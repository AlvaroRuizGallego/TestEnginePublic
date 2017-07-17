#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "State.h"
#include "Component.h"

#include <functional>


#define MEMBER_FUNCTION_AS_CONDITION(function) [&]()->bool { return function(); }

using Condition = std::function<bool()>;

struct StateNode
{
	State* state;
	std::map<StateNode*, Condition> childNodes;
};

class __declspec(dllexport) StateMachine: public Component
{
	private:
		void changeCurrentStateNode(StateNode* stateNode);

	protected:
		StateNode* _currentStateNode;

	public:
		void Update(double deltaTime) override;
};

#endif

