#include "ControllerInput.h"

#include "_InputManager.h"

#include "glm.hpp"


ControllerInput* ControllerInput::_instance = nullptr;


ControllerInput::ControllerInput()
{
}

ControllerInput::~ControllerInput()
{
	_instance = nullptr;
}

ControllerInput * ControllerInput::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new ControllerInput();
	}

	return _instance;
}

float ControllerInput::GetAxis(int controller, int axis)
{
	return glm::clamp(_InputManager::GetInstance()->GetAxis(controller, axis), -1.0f, 1.0f);
}

bool ControllerInput::IsControllerConnected(int controller)
{
	return _InputManager::GetInstance()->IsControllerConnected(controller);
}
