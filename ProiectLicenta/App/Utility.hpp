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

bool renderUI(Renderer renderer, GLFWwindow* window, bool &render_item, unsigned int &render_item_index)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	{
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", nullptr, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();

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
			return true;

		ImGui::Text("Performanta medie a aplicatiei %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	if (new_item_frame)
		drawNewItemFrame(renderer);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return false;
}