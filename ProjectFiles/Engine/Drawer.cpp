#include "Drawer.h"


Drawer* Drawer::_instance = nullptr;

Drawer::Drawer()
{
}

std::vector<glm::vec4>* Drawer::getVertices()
{
	_vertices.clear();

	for (auto it = _lineMap.begin(); it != _lineMap.end(); ++it)
	{
		std::vector<Line> * lines = &it->second;
		for (int i = 0; i < lines->size(); ++i)
		{
			_vertices.push_back(lines->at(i).points[0]);
			_vertices.push_back(lines->at(i).points[1]);
		}
	}

	return &_vertices;
}

std::vector<glm::vec4>* Drawer::getColors()
{
	_colors.clear();

	for (auto it = _lineMap.begin(); it != _lineMap.end(); ++it)
	{
		std::vector<Line> * lines = &it->second;
		for (int i = 0; i < lines->size(); ++i)
		{
			_colors.push_back(lines->at(i).color);
			_colors.push_back(lines->at(i).color);
		}
	}

	return &_colors;
}



Drawer::~Drawer()
{
	_instance = nullptr;
}

Drawer * Drawer::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new Drawer();
	}

	return _instance;
}

void Drawer::DrawLine(glm::vec2 pointA, glm::vec2 pointB, glm::vec4 color, int width)
{
	glm::vec4 pA = glm::vec4(pointA.x, pointA.y, 0, 1);
	glm::vec4 pB = glm::vec4(pointB.x, pointB.y, 0, 1);

	_lineMap[width].push_back({ pA, pB, color });
}

void Drawer::DrawCross(glm::vec2 pointA, float size, bool rotated, glm::vec4 color, int width)
{
	if (rotated)
	{
		DrawLine(pointA - glm::vec2(size*0.5, size*0.5), pointA + glm::vec2(size*0.5, size*0.5), color, width);
		DrawLine(pointA - glm::vec2(size*0.5, -size*0.5), pointA + glm::vec2(size*0.5, -size*0.5), color, width);
	}
	else
	{
		DrawLine(pointA - glm::vec2(size*0.5, 0), pointA + glm::vec2(size*0.5, 0), color, width);
		DrawLine(pointA - glm::vec2(0, size*0.5), pointA + glm::vec2(0, size*0.5), color, width);
	}
}

void Drawer::DrawCircle(glm::vec2 point, float radius, glm::vec4 color, int lineWidth)
{
	float angle = 10;

	for (float i = 0; i < 360; i += angle)
	{
		DrawLine(point + glm::vec2(cos(glm::radians(i)) * radius, sin(glm::radians(i)) * radius), point + glm::vec2(cos(glm::radians(i + angle)) * radius, sin(glm::radians(i + angle)) * radius), color, lineWidth);
	}
}

void Drawer::Clear()
{
	_lineMap.clear();
	_vertices.clear();
	_colors.clear();
}
