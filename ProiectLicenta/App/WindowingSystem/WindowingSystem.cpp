#include "WindowingSystem.hpp"
#include "SOIL2.h"

WindowingSystem::WindowingSystem()
{
	HDC hdc = GetDC(NULL);
	m_client_width = GetDeviceCaps(hdc, DESKTOPHORZRES);
	m_client_height = GetDeviceCaps(hdc, DESKTOPVERTRES);

	m_window = glfwCreateWindow(m_client_width, m_client_height, "GEOMAP", NULL, NULL);

	GLFWimage icons[1] = {};
	icons[0].pixels = SOIL_load_image("Miscellaneous/Logo/geomap_logo.png", &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
	glfwSetWindowIcon(m_window, 1, icons);
	SOIL_free_image_data(icons[0].pixels);
}

WindowingSystem::~WindowingSystem()
{
	glfwDestroyWindow(m_window);
}

int WindowingSystem::getScreenWidth() const
{
	return this->m_client_width;
}

void WindowingSystem::setScreenWidth(int in_width)
{
	this->m_client_width = in_width;
}

int WindowingSystem::getScreenHeight() const
{
	return this->m_client_height;
}

void WindowingSystem::setScreenHeight(int in_height)
{
	this->m_client_height = in_height;
}

GLFWwindow* WindowingSystem::getWindow() const
{
	return this->m_window;
}

void WindowingSystem::setWindow(GLFWwindow& new_window)
{
	this->m_window = &new_window;
}
