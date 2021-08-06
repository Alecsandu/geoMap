#include <iostream>
#include <exception>
#include <windows.h>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include "Renderer.hpp"
#include "WindowingSystem/WindowingSystem.hpp"
#include "glm.hpp"  
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Shader/ShaderLoader.hpp"
#include "ObjectLoader/ObjectLoader.hpp"
#include "Texture/Texture.hpp"
#include "Controls/Controls.hpp"
#include "VertexArray.hpp"
#include "gui/imgui.h"
#include "gui/imgui_impl_glfw.h"
#include "gui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <SOIL2.h>

bool new_item_frame = false;

void drawNewItemFrame(Renderer& renderer)
{
	ImGui::Begin("Item nou");

	ImGui::Text("Introduceti numele noii proiectii si al texturii(.obj si .dds):");  // Afiseaza text

	static char obj_name[128] = { 0 }, dds_name[128] = { 0 };
	ImGui::InputText("Nume fisier obj", &obj_name[0], 127);
	ImGui::InputText("Nume fisier dds", &dds_name[0], 127);
	if (ImGui::Button("Save"))
	{
		std::string name1 = std::string(obj_name),
			name2 = std::string(dds_name);
		std::cout << name1 << " " << name2 << "\n";
		renderer.addImage(name1.c_str(), name2.c_str());
		new_item_frame = false;
	}
	ImGui::Text("Performanta medie a aplicatiei %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void centerWindow(GLFWwindow* window, GLFWmonitor* monitor)
{
	if (!monitor)
		return;

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if (!mode)
		return;

	int monitorX, monitorY;
	glfwGetMonitorPos(monitor, &monitorX, &monitorY);

	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	glfwSetWindowPos(window,
		monitorX + (mode->width - windowWidth) / 2,
		monitorY + (mode->height - windowHeight) / 2);
}

void start_app()
{
	if (!glfwInit())
		throw std::exception("Failed to initialize GLFW!");

	glfwWindowHint(GLFW_DECORATED, 0);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* splash_screen = glfwCreateWindow(320, 240, "Undecorated Resizable", 0, 0);
	glfwMakeContextCurrent(splash_screen);

	glewExperimental = true;  // necesar pentru glfw core profile
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		throw std::exception("Failed to initialize GLEW!");
	}

	/* incarcarea imaginii ca o textura OpenGL */
	const char* logo_path = "Miscellaneous/Logo/resizedLogo.png";
	unsigned int logo_tex, vao_id, vbo_id, ebo_id;
	int width, height;
	unsigned char* image;
	glGenTextures(1, &logo_tex);
	glBindTexture(GL_TEXTURE_2D, logo_tex);
	image = SOIL_load_image(logo_path, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	ShaderLoader shader("Shaders/splash_screen_vertex_shader.vert", "Shaders/splash_screen_fragment_shader.frag");

	float varfuri[] = {
		 // Coordonatele varfurilor      // Coordonate de texturare
		-320.0f, -240.0f, 0.0f, 1.0f,    0.0f, 0.0f,
		 320.0f, -240.0f, 0.0f, 1.0f,    1.0f, 0.0f,
		 320.0f,  240.0f, 0.0f, 1.0f,    1.0f, 1.0f,
		-320.0f,  240.0f, 0.0f, 1.0f,    0.0f, 1.0f
	};
	unsigned int indici[] = {
		0, 1, 2,
		0, 2, 3
	};
	glGenVertexArrays(1, &vao_id);
	glGenBuffers(1, &vbo_id);
	glGenBuffers(1, &ebo_id);
	glBindVertexArray(vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(varfuri), varfuri, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indici), indici, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));

	auto start_time = std::chrono::high_resolution_clock::now();
	auto current_time = std::chrono::high_resolution_clock::now();

	centerWindow(splash_screen, glfwGetPrimaryMonitor());
	do
	{
		shader.Bind();
		/* activarea texturii */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, logo_tex);
		shader.set_uniform_matrix_4fv("mvp", glm::ortho(-320.0f, 320.0f, -240.0f, 240.0f, 0.0f, 1000.0f) * 
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		shader.set_uniform_1i("myTexture", 0);
		current_time = std::chrono::high_resolution_clock::now();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		shader.Unbind();
		glfwSwapBuffers(splash_screen);
		glfwPollEvents();
	} while (std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() < 2);
	glfwDestroyWindow(splash_screen);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo_id);
	glDeleteBuffers(1, &ebo_id);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao_id);
	shader.cleanup();
	glBindTexture(GL_TEXTURE_2D, 0);
	glfwTerminate();
}

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
	static std::string name = "";
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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::Begin("Meniu principal");
			ImGui::Text("Alegeti o proiectie:");

			for (unsigned int i = 0; i < renderer.getFilesNames().size(); ++i)
			{
				if (ImGui::Button(renderer.getFilesNames()[i].c_str()))
				{
					render_item = true;
					render_item_index = i;
				}
			}
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			if (ImGui::Button("Adauga o noua proiectie"))
				new_item_frame = true;

			if (ImGui::Button("Opreste afisarea"))
				render_item = false;

			if (glfwGetKey(window, GLFW_KEY_S))
			{
				render_item = true;
				render_item_index = 0;
			}

			if (ImGui::Button("Exit"))
				break;

			ImGui::Text("Performanta medie a aplicatiei %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (new_item_frame)
			drawNewItemFrame(renderer);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

int main()
{
	try
	{
		start_app();
		run_app();
	}
	catch (std::exception e)
	{
		std::cout << e.what() << "\n";
	}
	return 0;
}
