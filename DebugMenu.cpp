#include"DebugMenu.h"

DebugMenu::DebugMenu()
{
}

DebugMenu::DebugMenu(GLFWwindow* glfwWindow)
{
	window = glfwWindow;
	glfwGetWindowSize(glfwWindow, &windowWidth, &windowHeight);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

DebugMenu::~DebugMenu()
{
	//delete pickingRenderer;
}

void DebugMenu::create_menu(std::vector<GameObject>& entities, Camera camera, float deltaTime)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	std::string fps = std::to_string((int)(1.0f / deltaTime));
	fps = "fps = " + fps;
	char const* fpsChar = fps.c_str();

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_Always);

	ImGui::Begin("Debug Menu");
	ImGui::Text(fpsChar);

	//for (int i = 0; i < entities.size(); i++)
	//{
	//	GameObject& entity = entities[i];

	//	ImGui::Text("Entity %zu", i);

	//	ImGui::Text("Position:");
	//	//ImGui::Text("X: %.2f", entity.Position.x);
	//	//ImGui::Text("Y: %.2f", entity.Position.y);
	//	//ImGui::Text("Z: %.2f", entity.Position.z);
	//	ImGui::InputFloat(("X: %.2f" + std::to_string(i)).c_str(), &entity.Position.x, 1.0f, 1000.0f, "%.1f");
	//	ImGui::InputFloat(("Y: %.2f" + std::to_string(i)).c_str(), &entity.Position.y, 1.0f, 1000.0f, "%.1f");
	//	ImGui::InputFloat(("Z: %.2f" + std::to_string(i)).c_str(), &entity.Position.z, 1.0f, 1000.0f, "%.1f");

	//	ImGui::Text("Size:");
	//	ImGui::InputFloat(("Size X##" + std::to_string(i)).c_str(), &entity.Size.x, 1.0f, 100.0f, "%.1f");
	//	ImGui::InputFloat(("Size Y##" + std::to_string(i)).c_str(), &entity.Size.y, 1.0f, 100.0f, "%.1f");
	//	ImGui::InputFloat(("Size Z##" + std::to_string(i)).c_str(), &entity.Size.z, 1.0f, 100.0f, "%.1f");

	//	ImGui::Text("Rotation:");
	//	ImGui::InputFloat(("Rotation X##" + std::to_string(i)).c_str(), &entity.Rotation.x, 1.0f, 180.0f, "%.1f");
	//	ImGui::InputFloat(("Rotation Y##" + std::to_string(i)).c_str(), &entity.Rotation.y, 1.0f, 180.0f, "%.1f");
	//	ImGui::InputFloat(("Rotation Z##" + std::to_string(i)).c_str(), &entity.Rotation.z, 1.0f, 180.0f, "%.1f");

	//	ImGui::Separator();
	//}
	
	if (selectedIndex != -1)
	{
		GameObject& entity = entities[selectedIndex];

		ImGui::Text("Entity %zu", selectedIndex);

		ImGui::Text("Position:");
		//ImGui::Text("X: %.2f", entity.Position.x);
		//ImGui::Text("Y: %.2f", entity.Position.y);
		//ImGui::Text("Z: %.2f", entity.Position.z);
		ImGui::InputFloat(("X: %.2f" + std::to_string(selectedIndex)).c_str(), &entity.Position.x, 1.0f, 1000.0f, "%.1f");
		ImGui::InputFloat(("Y: %.2f" + std::to_string(selectedIndex)).c_str(), &entity.Position.y, 1.0f, 1000.0f, "%.1f");
		ImGui::InputFloat(("Z: %.2f" + std::to_string(selectedIndex)).c_str(), &entity.Position.z, 1.0f, 1000.0f, "%.1f");

		ImGui::Text("Size:");
		ImGui::InputFloat(("Size X##" + std::to_string(selectedIndex)).c_str(), &entity.Size.x, 1.0f, 100.0f, "%.1f");
		ImGui::InputFloat(("Size Y##" + std::to_string(selectedIndex)).c_str(), &entity.Size.y, 1.0f, 100.0f, "%.1f");
		ImGui::InputFloat(("Size Z##" + std::to_string(selectedIndex)).c_str(), &entity.Size.z, 1.0f, 100.0f, "%.1f");

		ImGui::Text("Rotation:");
		ImGui::InputFloat(("Rotation X##" + std::to_string(selectedIndex)).c_str(), &entity.Rotation.x, 1.0f, 180.0f, "%.1f");
		ImGui::InputFloat(("Rotation Y##" + std::to_string(selectedIndex)).c_str(), &entity.Rotation.y, 1.0f, 180.0f, "%.1f");
		ImGui::InputFloat(("Rotation Z##" + std::to_string(selectedIndex)).c_str(), &entity.Rotation.z, 1.0f, 180.0f, "%.1f");

		ImGui::Separator();
	}


	ImGui::Text("Camera Position:");
	ImGui::Text("X: %.2f", camera.getCameraPos().x);
	ImGui::Text("Y: %.2f", camera.getCameraPos().y);
	ImGui::Text("Z: %.2f", camera.getCameraPos().z);

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugMenu::shut_down()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void DebugMenu::select_object(std::vector<GameObject>& entities, Camera camera, float xpos, float ypos)
{

	Shader pickingShaderProgram("picking.vert.glsl", "picking.frag.glsl");
	Renderer pickingRenderer = Renderer(pickingShaderProgram);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//-------duplicate fix eventually
	glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 10000.0f);
	//
	pickingRenderer.shader.use();
	glEnableVertexAttribArray(0);
	glUniformMatrix4fv(glGetUniformLocation(pickingRenderer.shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(pickingRenderer.shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	
	for (int i = 0; i < entities.size(); i++)
	{
		int r = (i & 0x000000FF) >> 0;
		int g = (i & 0x0000FF00) >> 8;
		int b = (i & 0x00FF0000) >> 16;

		pickingRenderer.shader.setVec4("PickingColor", glm::vec4((float)r / 255.0, (float)g / 255.0, (float)b / 255.0, 1.0f));
		entities.at(i).draw(pickingRenderer);
		glGetError();
	}

	glDisableVertexAttribArray(0);
	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char data[4];
	std::cout << windowHeight << "\n";
	glReadPixels(xpos, windowHeight - ypos - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	int pickedID = data[0] + (data[1] * 256) + (data[2] * 256 * 256);

	std::cout << xpos << ", " << ypos << std::endl;

	if (!(pickedID == 0x00ffffff))
	{
		std::ostringstream oss;
		oss << "mesh " << pickedID;
		std::cout << oss.str() << std::endl;

		selectedIndex = pickedID;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int DebugMenu::get_selected_index()
{
	return selectedIndex;
}

void DebugMenu::deselect_index()
{
	selectedIndex = -1;
}