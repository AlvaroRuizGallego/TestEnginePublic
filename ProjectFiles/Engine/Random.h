#ifndef ENGINE_RANDOM_H
#define ENGINE_RANDOM_H

#include <limits>
#include <chrono>

class __declspec(dllexport) Random
{
	public:
		static void Seed()
		{
			srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		}

		static float Range(float minValue, float maxValue)//[minValue, maxValue)
		{
			 return Range01() * (maxValue - minValue) + minValue;
		}

		static int Range(int minValue, int maxValue)//[minValue, maxValue]
		{
			return (int)Range((float)minValue, (float)maxValue + 1.0f);
		}

		static float Range01()//[0, 1)
		{
			return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		}
};

#endif

