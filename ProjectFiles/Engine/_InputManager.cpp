#include "_InputManager.h"


_InputManager* _InputManager::_instance = nullptr;
std::queue<_InputManager::ButtonTrigger> _InputManager::_triggeredButtons;
std::queue<_InputManager::MouseButtonTrigger> _InputManager::_triggeredMouseButtons;
int _InputManager::_controllerStatus[MAX_CONTROLLERS];
unsigned char* _InputManager::_controllerPressedButtons[MAX_CONTROLLERS];
double _InputManager::_mouseX = 0;
double _InputManager::_mouseY = 0;


_InputManager::~_InputManager()
{
	_instance = nullptr;

	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		delete[] _controllerPressedButtons[i];
	}
}

_InputManager::_InputManager()
{
	_window = nullptr;

	glfwSetJoystickCallback(&_InputManager::controllerCallbackGLFW);
	checkAlreadyConnectedControllers();


	//Default callbacks (do nothing)
	_buttonCallback = [](int a, int b) -> void {};
	_controllerButtonCallback = [](int a, int b, int c) -> void {};
	_mouseButtonCallback = [](int a, int b, double c, double d) -> void {};
	_mouseMoveCallback = [](double a, double b) -> void {};
}

void _InputManager::keyCallbackGLFW(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	_triggeredButtons.push(ButtonTrigger(key, action));
}

void _InputManager::mouseButtonCallbackGLFW(GLFWwindow * window, int button, int action, int mods)
{
	_triggeredMouseButtons.push(MouseButtonTrigger(button, action, _mouseX, _mouseY));
}

void _InputManager::controllerCallbackGLFW(int controller, int status)
{
	int controllerIndex = controller - GLFW_JOYSTICK_1;//To get array position

	_controllerStatus[controllerIndex] = status;

	if (status == GLFW_CONNECTED)
	{
		printf("\nCONTROLLER %i: CONNECTED", controllerIndex);

		int count;
		glfwGetJoystickButtons(controllerIndex, &count);
		_controllerPressedButtons[controllerIndex] = new unsigned char[count];

		for (int i = 0; i < count; i++)
		{
			_controllerPressedButtons[controllerIndex][i] = GLFW_RELEASE;
		}
	}
	else if (status == GLFW_DISCONNECTED)
	{
		printf("\nCONTROLLER %i: DISCONNECTED", controllerIndex);

		delete[] _controllerPressedButtons[controllerIndex];
		_controllerPressedButtons[controllerIndex] = nullptr;
	}
}


void _InputManager::updateMousePosition()
{
	double currentMouseX, currentMouseY;
	bool mouseMoved = false;

	glfwGetCursorPos(_window->GetGLFWwindow(), &currentMouseX, &currentMouseY);

	if (currentMouseX != _mouseX)
	{
		_mouseX = currentMouseX;
		mouseMoved = true;
	}
	if (currentMouseY != _mouseY)
	{
		_mouseY = currentMouseY;
		mouseMoved = true;
	}

	if (mouseMoved)
	{
		_mouseMoveCallback(_mouseX, _mouseY);
	}
}

void _InputManager::checkAlreadyConnectedControllers()
{
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		controllerCallbackGLFW(GLFW_JOYSTICK_1 + i, glfwJoystickPresent(GLFW_JOYSTICK_1 + i) == GLFW_TRUE ? GLFW_CONNECTED : GLFW_DISCONNECTED);
	}
}

void _InputManager::updateControllerInput()
{
	for (int controllerIndex = 0; controllerIndex < MAX_CONTROLLERS; controllerIndex++)
	{
		if (_controllerStatus[controllerIndex] == GLFW_CONNECTED)
		{
			int count;
			const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1 + controllerIndex, &count);

			for (int buttonIndex = 0; buttonIndex < count; buttonIndex++)
			{
				if (buttonIndex >= MAX_CONTROLLER_BUTTONS) break;

				if (_controllerPressedButtons[controllerIndex][buttonIndex] != buttons[buttonIndex])
				{
					_controllerButtonCallback(CONTROLLER_1 + controllerIndex, INPUT_CONTROLLER_BUTTON_1 + buttonIndex, buttons[buttonIndex]);
					_controllerPressedButtons[controllerIndex][buttonIndex] = buttons[buttonIndex];
				}
			}
		}
	}
}

_InputManager * _InputManager::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new _InputManager();
	}
	return _instance;
}

void _InputManager::Update()
{
	updateMousePosition();
	updateControllerInput();

	while (_triggeredButtons.size() > 0)
	{
		ButtonTrigger buttonTrigger = _triggeredButtons.front();
		_buttonCallback(buttonTrigger.button, buttonTrigger.action);
		_triggeredButtons.pop();
	}

	while (_triggeredMouseButtons.size() > 0)
	{
		MouseButtonTrigger mouseButtonTrigger = _triggeredMouseButtons.front();
		_mouseButtonCallback(mouseButtonTrigger.button, mouseButtonTrigger.action, mouseButtonTrigger.x, mouseButtonTrigger.y);
		_triggeredMouseButtons.pop();
	}
}

void _InputManager::SetWindow(_Window * window)
{
	_window = window;

	glfwSetKeyCallback(_window->GetGLFWwindow(), &_InputManager::keyCallbackGLFW);
	glfwSetMouseButtonCallback(_window->GetGLFWwindow(), &_InputManager::mouseButtonCallbackGLFW);
}

void _InputManager::SetButtonCallback(ButtonCallback buttonCallback)
{
	_buttonCallback = buttonCallback;
}

void _InputManager::SetControllerButtonCallback(ControllerButtonCallback controllerButtonCallback)
{
	_controllerButtonCallback = controllerButtonCallback;
	_controllerButtonCallbackSet = true;
}

void _InputManager::SetMouseButtonCallback(MouseButtonCallback mouseButtonCallback)
{
	_mouseButtonCallback = mouseButtonCallback;
}

void _InputManager::SetMouseMoveCallback(MouseMoveCallback mouseMoveCallback)
{
	_mouseMoveCallback = mouseMoveCallback;
}

float _InputManager::GetAxis(int controller, int axis)
{
	int controllerIndex = controller - CONTROLLER_1;
	int axisIndex = axis - INPUT_CONTROLLER_AXIS_1;

	if (_controllerStatus[controllerIndex] == GLFW_CONNECTED)
	{
		int count;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1 + controllerIndex, &count);

		if (count > axisIndex)
		{
			return axes[axisIndex];
		}

		printf("\nWARNING: _InputManager::GetAxis -> There is no axis %i at the controller %i", axis, controller);
	}
	else
	{
		printf("\nWARNING: _InputManager::GetAxis -> Controller %i not connected", controller);
	}

	return 0;
}

bool _InputManager::IsControllerConnected(int controller)
{
	int controllerIndex = controller - CONTROLLER_1;

	return _controllerStatus[controllerIndex] == GLFW_CONNECTED;
}
