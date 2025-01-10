#include "Game.h"

bool isWireFrame = false;

Game::Game(GLFWwindow* window) : State(GAME_ACTIVE)
{
	inputManager = new InputManager(window);
}

Game::~Game()
{
}

void Game::init()
{
	AssetManager::load();

	Maps.push_back(new BoardMap("test_map_1"));
	Maps.push_back(new PongMap("test_map_2"));
	level = 0;

	Maps[level]->load();
}

void Game::update(float deltaTime)
{
	//if (Maps[level]->loadState == Map::LoadState::CHANGE_MAP)
	//{
	//	level++;
	//	Maps[level]->load();
	//}
	Maps[level]->update(deltaTime);
}

void Game::process_input(float deltaTime)
{
	Maps[level]->process_input(inputManager, deltaTime);

	if (State == GAME_ACTIVE || State == DEBUG_MENU)
	{
		if (inputManager->Keys[GLFW_KEY_M] && !inputManager->KeysProcessed[GLFW_KEY_M])
		{
			if (State == DEBUG_MENU)
			{
				State = GAME_ACTIVE;
			}
			else {
				State = DEBUG_MENU;
			}
			inputManager->KeysProcessed[GLFW_KEY_M] = true;
		}

		if (State == DEBUG_MENU)
		{
			if (inputManager->Keys[GLFW_KEY_P])
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