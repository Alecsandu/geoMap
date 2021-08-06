#pragma once
#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class WindowingSystem
{
public:
	WindowingSystem();
	~WindowingSystem();

	int getScreenWidth() const;
	void setScreenWidth(int);

	int getScreenHeight() const;
	void setScreenHeight(int);

	GLFWwindow* getWindow() const;
	void setWindow(GLFWwindow&);

private:
	int m_client_width;
	int m_client_height;
	GLFWwindow* m_window;
};

