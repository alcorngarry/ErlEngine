#include "Game.h"

Game::Game(GLFWwindow* window) : State(GAME_ACTIVE)
{
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
	
	if (State == DEBUG_MENU)
	{
		Maps[level]->state = Map::DEBUG;

		InputManager::set_key_binding(GLFW_KEY_W, new MoveCameraForwardCommand(Maps[level]->camera, deltaTime));
		InputManager::set_key_binding(GLFW_KEY_A, new MoveCameraLeftCommand(Maps[level]->camera, deltaTime));
		InputManager::set_key_binding(GLFW_KEY_S, new MoveCameraBackwardCommand(Maps[level]->camera, deltaTime));
		InputManager::set_key_binding(GLFW_KEY_D, new MoveCameraRightCommand(Maps[level]->camera, deltaTime));
		
		InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_LEFT, new SelectEntityCommand(InputManager::get_xpos(), InputManager::get_ypos(), false));
		InputManager::set_mouse_binding(GLFW_MOUSE_BUTTON_RIGHT, new SelectEntityCommand(0.0f, 0.0f, true));
		InputManager::set_key_binding(GLFW_KEY_N, new AddRemoveEntityCommand(Maps[level], true));
		InputManager::set_key_binding(GLFW_KEY_R, new AddRemoveEntityCommand(Maps[level], false));


		if (Renderer::get_selected_index() != -1)
		{
			InputManager::set_key_binding(GLFW_KEY_LEFT, new MoveEntityCommand(Maps[level]->entities[Renderer::get_selected_index()], 0, 3.0f));
			InputManager::set_key_binding(GLFW_KEY_RIGHT, new MoveEntityCommand(Maps[level]->entities[Renderer::get_selected_index()], 0, -3.0f));
			InputManager::set_key_binding(GLFW_KEY_UP, new MoveEntityCommand(Maps[level]->entities[Renderer::get_selected_index()], 1, 3.0f));
			InputManager::set_key_binding(GLFW_KEY_DOWN, new MoveEntityCommand(Maps[level]->entities[Renderer::get_selected_index()], 1, -3.0f));
			InputManager::set_key_binding(GLFW_KEY_J, new MoveEntityCommand(Maps[level]->entities[Renderer::get_selected_index()], 2, 3.0f));
			InputManager::set_key_binding(GLFW_KEY_K, new MoveEntityCommand(Maps[level]->entities[Renderer::get_selected_index()], 2, -3.0f));
		}
		else {
			Maps[level]->camera->setCameraFront(glm::normalize(glm::vec3(cos(glm::radians(InputManager::get_yaw())) * cos(glm::radians(InputManager::get_pitch())), sin(glm::radians(InputManager::get_pitch())), sin(glm::radians(InputManager::get_yaw())) * cos(glm::radians(InputManager::get_pitch())))));
		}
	}
	else {
		Maps[level]->state = Map::DEFAULT;
	}
	
	if (Maps[level]->loadState == Map::LoadState::CHANGE_MAP)
	{
		level++;
		Maps[level]->load();
	}
	
	Maps[level]->update(deltaTime);
}

void Game::set_debug_controls(float deltaTime)
{
}

void Game::render(float deltaTime)
{
	Maps[level]->draw(deltaTime);

	if (State == DEBUG_MENU)
	{
		Renderer::create_menu(deltaTime);
	}
} 