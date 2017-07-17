#ifndef _WINDOW_H
#define _WINDOW_H

#include "WindowMode.h"
#include "glfw3.h"

#include <string>


class _Window
{
	private:
		GLFWwindow* _glfwWindow;

	public:

		_Window(int width, int height, std::string  title, WindowMode mode, int samples);
		~_Window();

		void SwapBuffers();
		void MakeContextCurrent();
		int GetWidth();
		int GetHeight();
		bool ShouldClose();
		GLFWwindow* GetGLFWwindow();
		void SetTitle(std::string  title);
};

#endif