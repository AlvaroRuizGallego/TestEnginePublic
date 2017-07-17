#ifndef TEST_STATE_MACHINE_H
#define TEST_STATE_MACHINE_H

#include "StateMachine.h"

class TestStateMachine: public StateMachine
{
	private:
		bool b;

		bool testState1ToTestState2Condition();
		bool testState2ToTestState1Condition();

	public:
		TestStateMachine();
		~TestStateMachine();

		void OnMouseButtonDown(int button, glm::vec2 position, glm::vec2 worldPosition) override;
};

#endif

