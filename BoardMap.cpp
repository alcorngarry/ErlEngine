#include"BoardMap.h"

BoardMap::BoardMap(std::string mapName) : Map(mapName)
{
}

void BoardMap::update(float deltaTime)
{
	// move this to player update method !!!
	if (players[0]->inMotion)
	{	
		players[0]->update(deltaTime);
		players[0]->move_player(get_board_positions());
	}
	if (players[1]->inMotion)
	{
		players[1]->move_player(get_board_positions());
		players[1]->update(deltaTime);
	}
	if (players[2]->inMotion)
	{
		players[2]->move_player(get_board_positions());
		players[2]->update(deltaTime);
	}
	if (players[3]->inMotion)
	{
		players[3]->move_player(get_board_positions());
		players[3]->update(deltaTime);
	}
}

void BoardMap::process_input(InputManager& inputManager, float deltaTime)
{
	menu_input(inputManager, deltaTime);
	
	if (inputManager.Keys[GLFW_KEY_SPACE])
	{
		if (players[0]->inMotion == false)
		{
			players[0]->inMotion = true;
			players[0]->start_move((float)glfwGetTime(), roll_dice());
		}
	}

	if (inputManager.Keys[GLFW_KEY_I])
	{
		if (players[1]->inMotion == false)
		{
			players[1]->inMotion = true;
			players[1]->start_move((float)glfwGetTime(), roll_dice());
		}
	}

	if (inputManager.Keys[GLFW_KEY_O])
	{
		if (players[2]->inMotion == false)
		{
			players[2]->inMotion = true;
			players[2]->start_move((float)glfwGetTime(), roll_dice());
		}
	}

	if (inputManager.Keys[GLFW_KEY_P])
	{
		if (players[3]->inMotion == false)
		{
			players[3]->inMotion = true;
			players[3]->start_move((float)glfwGetTime(), roll_dice());
		}
	}
}

float BoardMap::roll_dice()
{
	srand(time(0));
	int roll = rand() % 6;

	std::cout << "rolled " << roll + 1 << std::endl;
	return roll + 1;
}

void BoardMap::load_players()
{
	glm::vec3 startingPosition = get_board_positions()[0];
	for (int i = 0; i < 4; i++)
	{
		players.push_back(new Player(AssetManager::get_model(0), startingPosition, glm::vec3(2.0f), glm::vec3(0.0f)));
	}
}

std::vector<glm::vec3> BoardMap::get_board_positions()
{
	std::vector<glm::vec3> output;

	for (GameObject* entity : entities)
	{
		if (entity->id == 4)
		{
			output.push_back(entity->Position + glm::vec3(0, 1, 0));
		}
	}
	return output;
}