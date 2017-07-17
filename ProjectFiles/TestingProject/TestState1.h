#ifndef TEST_STATE_1_H
#define TEST_STATE_1_H

#include "State.h"

class TestState1 : public State
{
	public:
		void OnEnterState() override;
		void Update(double deltaTime) override;
		void OnExitState() override;
};

#endif

