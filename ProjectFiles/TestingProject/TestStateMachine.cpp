#include "TestStateMachine.h"

#include "TestState1.h"
#include "TestState2.h"

bool TestStateMachine::testState1ToTestState2Condition()
{
	return b;
}

bool TestStateMachine::testState2ToTestState1Condition()
{
	return !b;
}

TestStateMachine::TestStateMachine()
{
	StateNode* testState1Node = new StateNode();
	testState1Node->state = new TestState1();

	StateNode* testState2Node = new StateNode();
	testState2Node->state = new TestState2();


	//1
	testState1Node->childNodes[testState2Node] = MEMBER_FUNCTION_AS_CONDITION(testState1ToTestState2Condition);

	//2
	testState2Node->childNodes[testState1Node] = MEMBER_FUNCTION_AS_CONDITION(testState2ToTestState1Condition);



	//Set current state
	_currentStateNode = testState1Node;

	b = false;
}


TestStateMachine::~TestStateMachine()
{
}

void TestStateMachine::OnMouseButtonDown(int button, glm::vec2 position, glm::vec2 worldPosition)
{
	b = !b;
}
