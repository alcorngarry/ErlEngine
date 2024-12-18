#include "Game.h"

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
	Maps[level]->draw(deltaTime);
}


