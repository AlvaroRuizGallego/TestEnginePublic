#include "Timer.h"

#include "glfw3.h"


Timer* Timer::_instance = nullptr;

Timer::Timer()
{
}

Timer::~Timer()
{
	_instance = nullptr;
}

Timer * Timer::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new Timer();
	}

	return _instance;
}

double Timer::GetTime()
{
	double elapsedTime = glfwGetTime() - _previousUnscaledTime;
	double currentTime = _previousTime + elapsedTime * _timeScale;
	
	_previousTime = currentTime;
	_previousUnscaledTime = glfwGetTime();

	return currentTime;
}

double Timer::GetTimeUnscaled()
{
	return glfwGetTime();
}

double Timer::GetTimeScale()
{
	return _timeScale;
}

void Timer::SetTimeScale(double timeScale)
{
	_timeScale = timeScale;
}