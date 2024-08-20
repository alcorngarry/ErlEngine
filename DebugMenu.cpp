#include"DebugMenu.h"

DebugMenu::DebugMenu(GLFWwindow* glfwWindow)
{
	window = glfwWindow;
	isWireframe = false;
	isMenuOpen = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void DebugMenu::create_menu(Camera camera, glm::vec3 &objectPos, float deltaTime, float rotationY)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	process_menu_input();

	std::string fps = std::to_string((int)(1000.0f / deltaTime));
	fps = "fps = " + fps;
	char const* fpsChar = fps.c_str();

	ImGui::Begin("Open this window, Thanks McDonalds for giving me this opportunity");
	ImGui::Text(fpsChar);

	std::string objectX = std::to_string((int)objectPos.x);
	objectX = "object X = " + objectX;
	char const* objectXChar = objectX.c_str();
	ImGui::Text(objectXChar);

	ImGui::SliderFloat("XXXXXX", &objectPos.x, 0.0f, 1000.0f);

	std::string objectY = std::to_string((int)objectPos.y);
	objectY = "object Y = " + objectY;
	char const* objectYChar = objectY.c_str();
	ImGui::Text(objectYChar);

	std::string objectZ = std::to_string((int)objectPos.z);
	objectZ = "object Z = " + objectZ;
	char const* objectZChar = objectZ.c_str();
	ImGui::Text(objectZChar);

	std::string cameraX = std::to_string((int)camera.getCameraPos().x);
	cameraX = "camera X = " + cameraX;
	char const* cameraXChar = cameraX.c_str();
	ImGui::Text(cameraXChar);

	std::string cameraY = std::to_string((int)camera.getCameraPos().y);
	cameraY = "camera Y = " + cameraY;
	char const* cameraYChar = cameraY.c_str();
	ImGui::Text(cameraYChar);

	std::string cameraZ = std::to_string((int)camera.getCameraPos().z);
	cameraZ = "camera Z = " + cameraZ;
	char const* cameraZChar = cameraZ.c_str();
	ImGui::Text(cameraZChar);

	std::string rotation = std::to_string((int)rotationY);
	rotation = "rotation = " + rotation;
	char const* rotationChar = rotation.c_str();
	ImGui::Text(rotationChar);

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

void DebugMenu::process_menu_input()
{
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		if (isWireframe)
		{
			isWireframe = false;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			isWireframe = true;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}