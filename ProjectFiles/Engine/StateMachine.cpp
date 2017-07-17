#include "StateMachine.h"



void StateMachine::changeCurrentStateNode(StateNode * stateNode)
{
	if (_currentStateNode != nullptr)
	{
		_currentStateNode->state->OnExitState();
	}

	_currentStateNode = stateNode;

	_currentStateNode->state->OnEnterState();
}


void StateMachine::Update(double deltaTime)
{
	_currentStateNode->state->Update(deltaTime);

	for (auto it = _currentStateNode->childNodes.begin(); it != _currentStateNode->childNodes.end(); ++it)
	{
		if (it->second())//If condition == true
		{
			changeCurrentStateNode(it->first);
			break;
		}
	}
}
