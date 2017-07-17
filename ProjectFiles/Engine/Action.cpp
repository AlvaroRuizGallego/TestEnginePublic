#include "Action.h"


Action::Action()
{
}

Action::~Action()
{
}

int Action::AddCallback(Callback callback)
{
	_callbacks.emplace(_currentCallbackKey, callback);

	return _currentCallbackKey++;
}

void Action::RemoveCallback(int callbackKey)
{
	_callbacks.erase(callbackKey);
}

void Action::operator()()
{
	for (auto it = _callbacks.begin(); it != _callbacks.end(); ++it)
	{
		it->second();
	}
}