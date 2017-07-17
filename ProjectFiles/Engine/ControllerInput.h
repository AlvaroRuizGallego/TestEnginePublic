#ifndef CONTROLLER_INPUT_H
#define CONTROLLER_INPUT_H

class __declspec(dllexport) ControllerInput
{
	private:
		static ControllerInput* _instance;

		ControllerInput();

	public:
		~ControllerInput();
		static ControllerInput* GetInstance();

		float GetAxis(int controller, int axis);
		bool IsControllerConnected(int controller);
};

#endif

