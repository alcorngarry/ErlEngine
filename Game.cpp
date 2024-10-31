#include "Game.h"

Renderer *lightRenderer;
Renderer *modelRenderer;
GLFWwindow* gameWindow;

AssetManager assetManager;
DebugMenu debugMenu;

bool isWireFrame = false;
int selectedIndex = -1;

Game::Game(GLFWwindow* window) : State(GAME_ACTIVE)
{
	inputManager = InputManager(window);
}

Game::~Game()
{
	delete lightRenderer;
	delete modelRenderer;
}

void Game::init(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	gameWindow = window;

	Shader shaderProgram("default.vert.glsl", "default.frag.glsl");
	Shader lightShaderProgram("light.vert.glsl", "light.frag.glsl");
	
	lightRenderer = new Renderer(lightShaderProgram);
	modelRenderer = new Renderer(shaderProgram);

	assetManager = AssetManager();
	assetManager.load();

	Maps.push_back(new BoardMap("test_map_1", debugMenu));
	Maps.push_back(new PongMap("test_map_2", debugMenu));
	this->level = 0;

	Maps[level]->load(assetManager);

	debugMenu = DebugMenu(gameWindow);
}

void Game::update(float deltaTime)
{
	inputManager.update();
	Maps[level]->update();
	
	//Maps[level].get_ball().Position += glm::vec3(1.0f, 0.0f, 0.0f);
}

void Game::process_input(float deltaTime)
{
	Maps[level]->process_input(inputManager, deltaTime);

	if (this->State == GAME_ACTIVE || this->State == DEBUG_MENU)
	{
		if (inputManager.Keys[GLFW_KEY_M] && !inputManager.KeysProcessed[GLFW_KEY_M])
		{
			if (this->State == DEBUG_MENU)
			{
				this->State = GAME_ACTIVE;
			}
			else {
				this->State = DEBUG_MENU;
			}
			this->inputManager.KeysProcessed[GLFW_KEY_M] = true;
		}

		if (this->State == DEBUG_MENU)
		{
			if (this->inputManager.Keys[GLFW_KEY_P])
			{
				if (isWireFrame)
				{
					isWireFrame = false;
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				else {
					isWireFrame = true;
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
			}
		}
	}
}

void Game::render(float deltaTime)
{
	/*if (this->State == GAME_ACTIVE)
	{	*/
		glm::mat4 view = glm::lookAt(Maps[level]->camera.getCameraPos(), Maps[level]->camera.getCameraPos() + Maps[level]->camera.getCameraFront(), Maps[level]->camera.getCameraUp());
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 10000.0f);
		
		modelRenderer->shader.use();
		glUniformMatrix4fv(glGetUniformLocation(modelRenderer->shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(modelRenderer->shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		modelRenderer->shader.setVec3("viewPos", Maps[level]->camera.getCameraPos());
		modelRenderer->shader.setBool("selected", false);
		modelRenderer->shader.setVec3("lightPos", glm::vec3(5.0f, 0.0f, 0.0f));
		modelRenderer->shader.setVec3("lightColor", glm::vec3(1.0f));

		Maps[level]->draw(*modelRenderer, false, deltaTime);

		lightRenderer->shader.use();
		glUniformMatrix4fv(glGetUniformLocation(lightRenderer->shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(lightRenderer->shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		lightRenderer->shader.setVec3("lightColor", glm::vec3(1.0f));
		Maps[level]->draw(*lightRenderer, true, deltaTime);

		/*if (this->State == DEBUG_MENU)
		{
			debugMenu->create_menu(this->Maps[level], deltaTime);
		}*/
	//}
}


