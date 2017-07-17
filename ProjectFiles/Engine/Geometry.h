#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "glm.hpp"

struct __declspec(dllexport) Rectangle
{
	glm::vec2 min, max;

	Rectangle()
	{
		min = glm::vec2();
		max = glm::vec2();
	}

	Rectangle(float minX, float minY, float maxX, float maxY) 
	{
		min = glm::vec2(minX, minY);
		max = glm::vec2(maxX, maxY);
	};

	Rectangle(glm::vec2 min, glm::vec2 max)
	{
		this->min = min;
		this->max = max;
	}

	float GetWidth()
	{
		return max.x - min.x;
	}

	float GetHeight()
	{
		return max.y - min.y;
	}
};

struct __declspec(dllexport) Circle
{
	glm::vec2 center;
	float radius;

	Circle() 
	{
		center = glm::vec2();
		radius = 1;
	};

	Circle(glm::vec2 center, float radius)
	{
		Circle::center = center;
		Circle::radius = radius;
	}
};

#endif

