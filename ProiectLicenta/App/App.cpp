#include "App.hpp"

#include <iostream>
#include <exception>
#include <windows.h>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2.h>

#include "glm.hpp"  
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Renderer.hpp"
#include "WindowingSystem/WindowingSystem.hpp"
#include "Shader/ShaderLoader.hpp"
#include "ObjectLoader/ObjectLoader.hpp"
#include "Texture/Texture.hpp"
#include "Controls/Controls.hpp"
#include "VertexArray.hpp"

#include "gui/imgui.h"
#include "gui/imgui_impl_glfw.h"
#include "gui/imgui_impl_opengl3.h"

#include "Utility.hpp"

void run_app()
{
	if (!glfwInit())
		throw std::exception("Failed to initialize GLFW!");
	/* hints pentru crearea ferestrei */
	glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
	glfwWindowHint(GLFW_DECORATED, 1);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	WindowingSystem* ws = new WindowingSystem();
	GLFWwindow* window = ws->getWindow();

	if (window == NULL)
	{
		glfwTerminate();
		throw std::exception("Failed to open GLFW window!");
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;  // necesar pentru glfw core profile
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		throw std::exception("Failed to initialize GLEW!");
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);  // pentru a putea sa folosesc tasta Escape cand opresc aplicatia	
	glfwPollEvents();
	glfwSetCursorPos(window, static_cast<double>(ws->getScreenWidth()) / 2, static_cast<double>(ws->getScreenHeight()) / 2);  // mut cursorul in centrul ecranului cand porneste aplicatia

	/* Initializare Dear ImGui */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

	/* GUI PART */
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

	ImGui_ImplGlfw_InitForOpenGL(window, false);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);  // Dark blue background

	glEnable(GL_DEPTH_TEST);  // Enable depth test
	glDepthFunc(GL_LESS);  // Accept fragment if it closer to the camera than the former one
	glEnable(GL_CULL_FACE);  // Cull triangles which normal is not towards the camera

	ShaderLoader shader("Shaders/vertex_shader.vert", "Shaders/fragment_shader.frag");
	VertexArray va;
	Controls controls;  // Clasa Controls se ocupa cu manipularea matricelor proiectie, vizualizare si model aka MVP
	Renderer renderer;
	renderer.Init();
	bool render_item = false;
	unsigned int render_item_index = -1;

	do
	{
		renderer.Clear();
		va.Bind();
		shader.Bind();

		controls.computeMatricesFromInputs(window);

		shader.set_uniform_matrix_4fv("model", controls.getModelMatrix() * glm::rotate(glm::mat4(1.0f), 4.7123f, glm::vec3(0.0f, 1.0f, 0.0f)));
		shader.set_uniform_matrix_4fv("vizualizare", controls.getViewMatrix());
		shader.set_uniform_matrix_4fv("proiectie", controls.getProjectionMatrix());
		shader.set_uniform_1i("myTextureSampler", 0);

		/* randarea obiectului ales folosind functia Draw din clasa Renderer */
		if (render_item) {
			renderer.Draw(renderer.getVBOArray()[render_item_index].first.first,
				renderer.getVBOArray()[render_item_index].first.second,
				renderer.getVBOArray()[render_item_index].second.first,
				renderer.getVBOArray()[render_item_index].second.second);
		}

		bool closeAppFlag = renderUI(renderer, window, render_item, render_item_index);
		if (closeAppFlag)
			break;

		shader.Unbind();
		va.Unbind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	/* Functii care apeleaza metodele de oprire ale interfetei */
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	renderer.cleanup();
	va.cleanup();
	shader.cleanup();

	delete ws;

	glfwTerminate();
}

void App::start()
{
	try
	{
		//start_app();
		run_app();
	}
	catch (std::exception e)
	{
		std::cout << e.what() << "\n";
	}
}
