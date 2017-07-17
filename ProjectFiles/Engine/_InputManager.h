#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include "_Window.h"
#include "InputBinds.h"

#include <functional>
#include <queue>


using ButtonCallback = std::function<void(int button, int action)>;
using ControllerButtonCallback = std::function<void(int controller, int button, int action)>;
using MouseButtonCallback = std::function<void(int button, int action, double x, double y)>;
using MouseMoveCallback = std::function<void(double x, double y)>;


class _InputManager
{
	private:

		struct ButtonTrigger
		{
			int button, action;

			ButtonTrigger(int button, int action)
			{
				ButtonTrigger::button = button;
				ButtonTrigger::action = action;
			}
		};

		struct MouseButtonTrigger
		{
			double x, y;
			int button;
			int action;

			MouseButtonTrigger(int button, int action, double x, double y)
			{
				MouseButtonTrigger::button = button;
				MouseButtonTrigger::action = action;
				MouseButtonTrigger::x = x;
				MouseButtonTrigger::y = y;
			}
		};

		static _InputManager* _instance;

		_Window* _window;

		ButtonCallback _buttonCallback;
		ControllerButtonCallback _controllerButtonCallback;
		MouseButtonCallback _mouseButtonCallback;
		MouseMoveCallback _mouseMoveCallback;

		bool _controllerButtonCallbackSet = false;

		static double _mouseX, _mouseY;

		static std::queue<ButtonTrigger> _triggeredButtons;
		static std::queue<MouseButtonTrigger> _triggeredMouseButtons;

		static const int MAX_CONTROLLERS = (GLFW_JOYSTICK_LAST - GLFW_JOYSTICK_1) + 1;
		static const int MAX_CONTROLLER_BUTTONS = (INPUT_CONTROLLER_BUTTON_LAST - INPUT_CONTROLLER_BUTTON_1) + 1;

		static int _controllerStatus[MAX_CONTROLLERS];
		static unsigned char* _controllerPressedButtons[MAX_CONTROLLERS];


		_InputManager();

		static void keyCallbackGLFW(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseButtonCallbackGLFW(GLFWwindow* window, int button, int action, int mods);
		static void controllerCallbackGLFW(int controller, int status);

		void updateMousePosition();

		void checkAlreadyConnectedControllers();
		void updateControllerInput();

	public:
		~_InputManager();

		static _InputManager* GetInstance();

		void Update();
		void SetWindow(_Window* window);
		void SetButtonCallback(ButtonCallback buttonCallback);
		void SetControllerButtonCallback(ControllerButtonCallback controllerButtonCallback);
		void SetMouseButtonCallback(MouseButtonCallback mouseButtonCallback);
		void SetMouseMoveCallback(MouseMoveCallback mouseMoveCallback);

		float GetAxis(int controller, int axis);
		bool IsControllerConnected(int controller);
};

#endif
