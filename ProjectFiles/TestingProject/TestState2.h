#ifndef TEST_STATE_2_H
#define TEST_STATE_2_H

#include "State.h"

class TestState2: public State
{
	public:
		void OnEnterState() override;
		void Update(double deltaTime) override;
		void OnExitState() override;
};

#endif

