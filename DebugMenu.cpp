#include"DebugMenu.h"

DebugMenu::DebugMenu()
{

}

DebugMenu::DebugMenu(GLFWwindow* glfwWindow)
{
	window = glfwWindow;
	Shader pickingShaderProgram("picking.vert.glsl", "picking.frag.glsl");
	pickingRenderer = new Renderer(pickingShaderProgram);
	selectedIndex = -1;

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
	delete pickingRenderer;
}

void DebugMenu::create_menu(std::vector<GameObject> entities, Camera camera, float deltaTime)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	std::string fps = std::to_string((int)(1.0f / deltaTime));
	fps = "fps = " + fps;
	char const* fpsChar = fps.c_str();

	ImGui::Begin("Open this window, Thanks McDonalds for giving me this opportunity");
	ImGui::Text(fpsChar);

	for (GameObject entity : entities)
	{
		std::string objectX = std::to_string((int)entity.Position.x);
		objectX = "object X = " + objectX + "\n";
		char const* objectXChar = objectX.c_str();
		ImGui::Text(objectXChar);

		std::string objectY = std::to_string((int)entity.Position.y);
		objectY = "object Y = " + objectY;
		char const* objectYChar = objectY.c_str();
		ImGui::Text(objectYChar);

		std::string objectZ = std::to_string((int)entity.Position.z);
		objectZ = "object Z = " + objectZ;
		char const* objectZChar = objectZ.c_str();
		ImGui::Text(objectZChar);
	}


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

void DebugMenu::select_object(std::vector<GameObject> entities, Camera camera, float xpos, float ypos)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//-------duplicate fix eventually
	glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 10000.0f);
	//
	pickingRenderer->shader.use();
	glEnableVertexAttribArray(0);
	glUniformMatrix4fv(glGetUniformLocation(pickingRenderer->shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(pickingRenderer->shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	draw_picking(*pickingRenderer, entities);

	glDisableVertexAttribArray(0);
	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char data[4];
	glReadPixels(xpos, 768 - ypos - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

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

void DebugMenu::draw_picking(Renderer& renderer, std::vector<GameObject> entities)
{
	for (int i = 0; i < entities.size(); i++)
	{
		int r = (i & 0x000000FF) >> 0;
		int g = (i & 0x0000FF00) >> 8;
		int b = (i & 0x00FF0000) >> 16;

		renderer.shader.setVec4("PickingColor", glm::vec4((float)r / 255.0, (float)g / 255.0, (float)b / 255.0, 1.0f));
		entities.at(i).draw(renderer);
	}
}