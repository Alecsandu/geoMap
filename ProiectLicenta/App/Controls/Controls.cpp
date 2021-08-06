#include "controls.hpp"

Controls::Controls()
{
	m_position = glm::vec3(0, 0, 5);							// Pozitia initiala a camerei
	m_horizontal_angle = 3.14f;									// Unghiul orizontal initial : indreptat catre -Z
	m_vertical_angle = 0.0f;									// Unghiul vertical initial
	m_direction = {												// Directia initiala
		cos(m_vertical_angle)* sin(m_horizontal_angle),
		sin(m_vertical_angle),
		cos(m_vertical_angle)* cos(m_horizontal_angle)
	};				
	m_right = {
		sin(m_horizontal_angle - 3.14f / 2.0f),
		0,
		cos(m_horizontal_angle - 3.14f / 2.0f)
	};
	m_initial_FOV = 45.0f;										// Camp vizual initial
	m_speed = 4.0f;												// Viteza operatiilor de rotire/miscare a modelelor 4 units/second
	m_mouse_speed = 0.01f;										// Setarea vitezei pentru realizarea operatiilor folosind mouse-ul
	m_delta_time = 0.0f;

	m_matrice_proiectie = glm::mat4(1.0f);						// Matricea care se ocupa cu crearea efectului de perspectiva
	m_matrice_vizualizare = glm::mat4(1.0f);					//
	m_matrice_model = glm::mat4(1.0f);							//
}

Controls::~Controls()
{
}

/* Getter si setter pentru matricea proiectie */
glm::mat4 Controls::getProjectionMatrix()
{
	return this->m_matrice_proiectie;
}
void Controls::setProjectionMatrix(glm::mat4 new_projection_matrix)
{
	this->m_matrice_proiectie = new_projection_matrix;
}

/* Getter si setter pentru matricea vizualizare */
glm::mat4 Controls::getViewMatrix()
{
	return this->m_matrice_vizualizare;
}
void Controls::setViewMatrix(glm::mat4 new_view_matrix)
{
	this->m_matrice_vizualizare = new_view_matrix;
}

/* Getter si setter pentru matricea model */
glm::mat4 Controls::getModelMatrix()
{
	return this->m_matrice_model;
}
void Controls::setModelMatrix(glm::mat4 in_model_matrix)
{
	this->m_matrice_model = in_model_matrix;
}

void Controls::computeMatricesFromInputs(GLFWwindow* window)
{
	static double last_time = glfwGetTime();
	double current_time = glfwGetTime();
	m_delta_time = float(current_time - last_time);

	double x_pos = 1920.0 / 2,
		   y_pos = 1080.0 / 2;

	/* compute the new orientation */
	m_horizontal_angle += m_mouse_speed * float(1920 / 2 - x_pos);
	m_vertical_angle += m_mouse_speed * float(1080 / 2 - y_pos);

	/* Direction: Spherical coordinates to Cartesian coordinates conversion */
	m_direction = {
		cos(m_vertical_angle) * sin(m_horizontal_angle),
		sin(m_vertical_angle),
		cos(m_vertical_angle) * cos(m_horizontal_angle)
	};

	m_right = glm::vec3(
		sin(m_horizontal_angle - 3.14f / 2.0f),
		0,
		cos(m_horizontal_angle - 3.14f / 2.0f)
	);

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		m_position += m_direction * m_delta_time * m_speed;

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		m_position -= m_direction * m_delta_time * m_speed;

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		m_position += m_right * m_delta_time * m_speed;

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		m_position -= m_right * m_delta_time * m_speed;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		m_matrice_model *= glm::rotate(glm::mat4(1.0f), 1.0f * m_delta_time * m_speed, glm::vec3(0.0f, 1.0f, 0.0f));

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		m_matrice_model *= glm::rotate(glm::mat4(1.0f), -1.0f * m_delta_time * m_speed, glm::vec3(0.0f, 1.0f, 0.0f));

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		m_matrice_model *= glm::rotate(glm::mat4(1.0f), 1.0f * m_delta_time * m_speed, glm::vec3(1.0f, 0.0f, 0.0f));

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		m_matrice_model *= glm::rotate(glm::mat4(1.0f), -1.0f * m_delta_time * m_speed, glm::vec3(1.0f, 0.0f, 0.0f));

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		m_matrice_model = glm::mat4(1.0f);
		m_position = glm::vec3(0, 0, 5);
	}

	glm::vec3 up = glm::cross(m_right, m_direction);

	m_matrice_vizualizare = glm::lookAt(
		m_position,
		m_position + m_direction,
		up
	);

	m_matrice_proiectie = glm::perspective(m_initial_FOV, 4.0f / 3.0f, 0.1f, 100.0f);

	last_time = current_time;
}
