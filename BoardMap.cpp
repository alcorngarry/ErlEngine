#include"BoardMap.h"

BoardMap::BoardMap(std::string mapName) : Map(mapName)
{
}

void BoardMap::update(float deltaTime)
{
	if (players.at(0).inMotion)
	{	
		players.at(0).move_player(get_board_positions());

	/*	camera.setCameraPos(players.at(0).Position + glm::vec3(0.0f, 50.0f, 150.0f));
		camera.setCameraFront(players.at(0).Position);*/
	}
	if (players.at(1).inMotion)
	{
		players.at(1).move_player(get_board_positions());
		//camera.setCameraPos(players.at(1).Position + glm::vec3(0.0f, 30.0f, 50.0f));
		//camera.setCameraFront(players.at(1).Position);
	}
	if (players.at(2).inMotion)
	{
		players.at(2).move_player(get_board_positions());
		//camera.setCameraPos(players.at(2).Position + glm::vec3(0.0f, 30.0f, 50.0f));
		//camera.setCameraFront(players.at(2).Position);
	}
	if (players.at(3).inMotion)
	{
		players.at(3).move_player(get_board_positions());
		//camera.setCameraPos(players.at(3).Position + glm::vec3(0.0f, 30.0f, 50.0f));
		//camera.setCameraFront(players.at(3).Position * 5.0f);
	}
	//if (!players.at(0).inMotion && players.at(0).get_board_position() > 6)
	//{
	//	this->level = 1;
	//	//Maps[level].load(assetManager);
	//}
}

void BoardMap::process_input(InputManager& inputManager, float deltaTime)
{
	menu_input(inputManager, deltaTime);
	
	if (inputManager.Keys[GLFW_KEY_SPACE])
	{
		if (players.at(0).inMotion == false)
		{
			players.at(0).inMotion = true;
			players.at(0).start_move((float)glfwGetTime(), roll_dice() + 1);
		}
	}

	if (inputManager.Keys[GLFW_KEY_I])
	{
		if (players.at(1).inMotion == false)
		{
			players.at(1).inMotion = true;
			players.at(1).start_move((float)glfwGetTime(), roll_dice() + 1);
		}
	}

	if (inputManager.Keys[GLFW_KEY_O])
	{
		if (players.at(2).inMotion == false)
		{
			players.at(2).inMotion = true;
			players.at(2).start_move((float)glfwGetTime(), roll_dice() + 1);
		}
	}

	if (inputManager.Keys[GLFW_KEY_P])
	{
		if (players.at(3).inMotion == false)
		{
			players.at(3).inMotion = true;
			players.at(3).start_move((float)glfwGetTime(), roll_dice() + 1);
		}
	}

}

float BoardMap::roll_dice()
{
	srand(time(0));
	int roll = rand() % 6;

	std::cout << "rolled " << roll + 1 << std::endl;
	return roll;
}

void BoardMap::load_players(AssetManager assetManager)
{
	glm::vec3 startingPosition = get_board_positions()[0];
	/*player1 = new Player(*assetManager.get_model(0), startingPosition + glm::vec3(0, 6, 0), glm::vec3(2.0f));
	player2 = new Player(*assetManager.get_model(0), startingPosition + glm::vec3(0, 6, 0), glm::vec3(2.0f));
	player3 = new Player(*assetManager.get_model(0), startingPosition + glm::vec3(0, 6, 0), glm::vec3(2.0f));
	player4 = new Player(*assetManager.get_model(0), startingPosition + glm::vec3(0, 6, 0), glm::vec3(2.0f));*/
	for (int i = 0; i < 4; i++)
	{
		players.push_back(Player(*assetManager.get_model(0), startingPosition + glm::vec3(0, 6, 0), glm::vec3(2.0f)));
	}
}

std::vector<glm::vec3> BoardMap::get_board_positions()
{
	std::vector<glm::vec3> output;

	for (GameObject entity : this->entities)
	{
		if (entity.id == 4)
		{
			output.push_back(entity.Position);
		}
	}
	return output;
}