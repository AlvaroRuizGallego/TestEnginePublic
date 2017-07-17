#include "_Window.h"

#include <iostream>

_Window::_Window(int width, int height, std::string  title, WindowMode mode, int samples)
{
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, samples);
	
	switch (mode)
	{
		case WINDOWED:
			_glfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
			break;

		case FULLSCREEN:
			_glfwWindow = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
			break;
	}

	if (!_glfwWindow)
	{
		glfwTerminate();
		printf("\nERROR: _Window::_Window -> Window or OpenGL context creation failed");
		fflush(stdout);
		exit(EXIT_FAILURE);
	}
}


_Window::~_Window()
{
	glfwDestroyWindow(_glfwWindow);
}

void _Window::SwapBuffers()
{
	glfwSwapBuffers(_glfwWindow);
}

void _Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(_glfwWindow);
}

int _Window::GetWidth()
{
	int w, h;
	glfwGetWindowSize(_glfwWindow, &w, &h);

	return w;
}

int _Window::GetHeight()
{
	int w, h;
	glfwGetWindowSize(_glfwWindow, &w, &h);

	return h;
}

bool _Window::ShouldClose()
{
	return glfwWindowShouldClose(_glfwWindow);
}

GLFWwindow * _Window::GetGLFWwindow()
{
	return _glfwWindow;
}

void _Window::SetTitle(std::string  title)
{
	glfwSetWindowTitle(_glfwWindow, title.c_str());
}


