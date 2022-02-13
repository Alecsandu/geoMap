#ifndef CONTROLS_CLASS_H
#define CONTROLS_CLASS_H

#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Controls
{
public:
	Controls();
	~Controls() = default;

	glm::mat4 getProjectionMatrix();
	void setProjectionMatrix(glm::mat4);

	glm::mat4 getViewMatrix();
	void setViewMatrix(glm::mat4);

	glm::mat4 getModelMatrix();
	void setModelMatrix(glm::mat4);

	void computeMatricesFromInputs(GLFWwindow* window);

private:
	glm::mat4 m_matrice_proiectie;
	glm::mat4 m_matrice_vizualizare;
	glm::mat4 m_matrice_model;
	glm::vec3 m_position;
	
	float m_horizontal_angle;
	float m_vertical_angle;
	glm::vec3 m_direction;
	glm::vec3 m_right;
	float m_initial_FOV;

	float m_speed;
	float m_mouse_speed;
	float m_delta_time;
};

#endif CONTROLS_CLASS_H