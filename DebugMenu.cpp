#include"DebugMenu.h"

DebugMenu::DebugMenu()
{
}

DebugMenu::DebugMenu(GLFWwindow* glfwWindow)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

DebugMenu::~DebugMenu()
{
	//delete pickingRenderer;
}

void DebugMenu::create_menu(std::vector<GameObject*> entities, Camera* camera, float deltaTime, int roll)
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

	//view all objects in a scene
	//for (int i = 0; i < entities.size(); i++)
	//{
	//	GameObject* entity = entities[i];

	//	ImGui::Text("Entity %zu", i);

	//	ImGui::Text("Position:");
	//	ImGui::InputFloat(("X: ## " + std::to_string(i)).c_str(), &entity->Position.x, 1.0f, 1000.0f, "%.1f");
	//	ImGui::InputFloat(("Y: ##" + std::to_string(i)).c_str(), &entity->Position.y, 1.0f, 1000.0f, "%.1f");
	//	ImGui::InputFloat(("Z: ##" + std::to_string(i)).c_str(), &entity->Position.z, 1.0f, 1000.0f, "%.1f");

	//	ImGui::Text("Size:");
	//	ImGui::InputFloat(("Size X##" + std::to_string(i)).c_str(), &entity->Size.x, 1.0f, 100.0f, "%.1f");
	//	ImGui::InputFloat(("Size Y##" + std::to_string(i)).c_str(), &entity->Size.y, 1.0f, 100.0f, "%.1f");
	//	ImGui::InputFloat(("Size Z##" + std::to_string(i)).c_str(), &entity->Size.z, 1.0f, 100.0f, "%.1f");

	//	ImGui::Text("Rotation:");
	//	ImGui::InputFloat(("Rotation X##" + std::to_string(i)).c_str(), &entity->Rotation.x, 1.0f, 180.0f, "%.1f");
	//	ImGui::InputFloat(("Rotation Y##" + std::to_string(i)).c_str(), &entity->Rotation.y, 1.0f, 180.0f, "%.1f");
	//	ImGui::InputFloat(("Rotation Z##" + std::to_string(i)).c_str(), &entity->Rotation.z, 1.0f, 180.0f, "%.1f");

	//	ImGui::Separator();
	//}

	int selectedIndex = Renderer::get_selected_index();
	if (selectedIndex != -1)
	{
		GameObject* entity = entities[selectedIndex];

		ImGui::Text("Entity %zu", selectedIndex);

		ImGui::Text("Position:");
		ImGui::InputFloat(("X: %.2f" + std::to_string(selectedIndex)).c_str(), &entity->Position.x, 1.0f, 1000.0f, "%.1f");
		ImGui::InputFloat(("Y: %.2f" + std::to_string(selectedIndex)).c_str(), &entity->Position.y, 1.0f, 1000.0f, "%.1f");
		ImGui::InputFloat(("Z: %.2f" + std::to_string(selectedIndex)).c_str(), &entity->Position.z, 1.0f, 1000.0f, "%.1f");

		ImGui::Text("Size:");
		ImGui::InputFloat(("Size X##" + std::to_string(selectedIndex)).c_str(), &entity->Size.x, 1.0f, 100.0f, "%.1f");
		ImGui::InputFloat(("Size Y##" + std::to_string(selectedIndex)).c_str(), &entity->Size.y, 1.0f, 100.0f, "%.1f");
		ImGui::InputFloat(("Size Z##" + std::to_string(selectedIndex)).c_str(), &entity->Size.z, 1.0f, 100.0f, "%.1f");

		ImGui::Text("Rotation:");
		ImGui::InputFloat(("Rotation X##" + std::to_string(selectedIndex)).c_str(), &entity->Rotation.x, 1.0f, 180.0f, "%.1f");
		ImGui::InputFloat(("Rotation Y##" + std::to_string(selectedIndex)).c_str(), &entity->Rotation.y, 1.0f, 180.0f, "%.1f");
		ImGui::InputFloat(("Rotation Z##" + std::to_string(selectedIndex)).c_str(), &entity->Rotation.z, 1.0f, 180.0f, "%.1f");

		ImGui::Separator();
	}

	//if (ImGui::Button("Plane" + roll)) {
	//	// Code to execute when the button is clicked
	//	// For example, print a message or trigger an action
	//	printf("added plane\n");
	//}

	ImGui::Text("Camera Position:");
	ImGui::Text("X: %.2f", camera->getCameraPos().x);
	ImGui::Text("Y: %.2f", camera->getCameraPos().y);
	ImGui::Text("Z: %.2f", camera->getCameraPos().z);



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