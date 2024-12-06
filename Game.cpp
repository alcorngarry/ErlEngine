#include "Game.h"

GLFWwindow* gameWindow;

AssetManager assetManager;

bool isWireFrame = false;

Game::Game(GLFWwindow* window) : State(GAME_ACTIVE)
{
	inputManager = InputManager(window);
	//glfwGetWindowSize(window, &windowWidth, &windowHeight);
}

Game::~Game()
{
}

void Game::init()
{
	/*lightRenderer = new Renderer(lightShaderProgram);
	modelRenderer = new Renderer(shaderProgram);
	skinnedRenderer = new Renderer(skinnedShaderProgram);*/

	assetManager = AssetManager();
	assetManager.load();

	//debugMenu = DebugMenu(gameWindow);

	Maps.push_back(new BoardMap("test_map_1"));
	Maps.push_back(new PongMap("test_map_2"));
	this->level = 1;

	Maps[level]->load(assetManager);
}

void Game::update(float deltaTime)
{
	inputManager.update();
	Maps[level]->update(deltaTime);
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
		/*glm::mat4 view = glm::lookAt(Maps[level]->camera.getCameraPos(), Maps[level]->camera.getCameraPos() + Maps[level]->camera.getCameraFront(), Maps[level]->camera.getCameraUp());
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 10000.0f);

		modelRenderer->shader.use();
		glUniformMatrix4fv(glGetUniformLocation(modelRenderer->shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(modelRenderer->shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		modelRenderer->shader.setVec3("viewPos", Maps[level]->camera.getCameraPos());
		modelRenderer->shader.setBool("selected", false);
		modelRenderer->shader.setVec3("lightPos", Maps[level]->lights.at(0).Position);
		modelRenderer->shader.setVec3("lightColor", glm::vec3(1.0f));

		Maps[level]->draw(*modelRenderer, false, deltaTime);

		lightRenderer->shader.use();
		glUniformMatrix4fv(glGetUniformLocation(lightRenderer->shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(lightRenderer->shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		lightRenderer->shader.setVec3("lightColor", glm::vec3(1.0f));*/
		Maps[level]->draw(deltaTime);

		/*if (this->State == DEBUG_MENU)
		{
			debugMenu->create_menu(this->Maps[level], deltaTime);
		}*/
	//}
}


