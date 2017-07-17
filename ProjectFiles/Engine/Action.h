#ifndef ACTION_H
#define ACTION_H

#include <map>
#include <functional>

using Callback = std::function<void()>;

#define MEMBER_FUNCTION_AS_CALLBACK(function) [&]()->void { return function(); }

class __declspec(dllexport) Action
{
	private:
		std::map<int, Callback> _callbacks;
		int _currentCallbackKey = 0;
			
	public:
		Action();
		~Action();

		int AddCallback(Callback callback);
		void RemoveCallback(int callbackKey);
		
		void operator()();
};

#endif
