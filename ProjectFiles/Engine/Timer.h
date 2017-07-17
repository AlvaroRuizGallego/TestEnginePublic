#ifndef ENGINE_TIMER_H
#define ENGINE_TIMER_H


class __declspec(dllexport) Timer
{
	private:
		static Timer* _instance;

		double _timeScale = 1.0;
		double _previousTime = 0;
		double _previousUnscaledTime = 0;

		Timer();

	public:

		~Timer();

		static Timer* GetInstance();

		double GetTime();
		double GetTimeUnscaled();

		double GetTimeScale();
		void SetTimeScale(double timeScale);
};

#endif