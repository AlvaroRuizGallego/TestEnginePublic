#ifndef STATE_H
#define STATE_H

class __declspec(dllexport) State
{
	public:
		virtual void OnEnterState() = 0;
		virtual void Update(double deltaTime) = 0;
		virtual void OnExitState() = 0;
};

#endif

