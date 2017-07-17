#ifndef DRAWER_H
#define DRAWER_H

#include "glm.hpp"

#include <map>
#include <vector>


class __declspec(dllexport) Drawer
{
	friend class _RenderingEngine;
	
	private:

		struct Line
		{
			glm::vec4 points[2];
			glm::vec4 color;
		};

		static Drawer* _instance;

		std::map<int, std::vector<Line>> _lineMap;//Key: line width, Value: All lines with that width,
		std::vector<glm::vec4> _vertices;
		std::vector<glm::vec4> _colors;

		Drawer();

		std::vector<glm::vec4>* getVertices();
		std::vector<glm::vec4>* getColors();

	public:
		~Drawer();

		static Drawer* GetInstance();

		void DrawLine(glm::vec2 pointA, glm::vec2 pointB, glm::vec4 color = glm::vec4(1,1,1,1), int width = 2);
		void DrawCross(glm::vec2 point, float size, bool rotated = true, glm::vec4 color = glm::vec4(1, 1, 1, 1), int width = 2);
		void DrawCircle(glm::vec2 point, float radius, glm::vec4 color = glm::vec4(1,1,1,1), int lineWidth = 2);
		void Clear();

};

#endif
