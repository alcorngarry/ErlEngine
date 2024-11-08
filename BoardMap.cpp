#include"BoardMap.h"

BoardMap::BoardMap(std::string mapName, DebugMenu debugMenu) : Map(mapName, debugMenu)
{
}

void BoardMap::update(float deltaTime)
{
	if (player1->inMotion)
	{	
		player1->move_player(get_board_positions());

	/*	camera.setCameraPos(player1->Position + glm::vec3(0.0f, 50.0f, 150.0f));
		camera.setCameraFront(player1->Position);*/
	}
	if (player2->inMotion)
	{
		player2->move_player(get_board_positions());
		//camera.setCameraPos(player2->Position + glm::vec3(0.0f, 30.0f, 50.0f));
		//camera.setCameraFront(player2->Position);
	}
	if (player3->inMotion)
	{
		player3->move_player(get_board_positions());
		//camera.setCameraPos(player3->Position + glm::vec3(0.0f, 30.0f, 50.0f));
		//camera.setCameraFront(player3->Position);
	}
	if (player4->inMotion)
	{
		player4->move_player(get_board_positions());
		//camera.setCameraPos(player4->Position + glm::vec3(0.0f, 30.0f, 50.0f));
		//camera.setCameraFront(player4->Position * 5.0f);
	}
	//if (!player1->inMotion && player1->get_board_position() > 6)
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
		if (player1->inMotion == false)
		{
			player1->inMotion = true;
			player1->start_move((float)glfwGetTime(), roll_dice() + 1);
		}
	}

	if (inputManager.Keys[GLFW_KEY_I])
	{
		if (player2->inMotion == false)
		{
			player2->inMotion = true;
			player2->start_move((float)glfwGetTime(), roll_dice() + 1);
		}
	}

	if (inputManager.Keys[GLFW_KEY_O])
	{
		if (player3->inMotion == false)
		{
			player3->inMotion = true;
			player3->start_move((float)glfwGetTime(), roll_dice() + 1);
		}
	}

	if (inputManager.Keys[GLFW_KEY_P])
	{
		if (player4->inMotion == false)
		{
			player4->inMotion = true;
			player4->start_move((float)glfwGetTime(), roll_dice() + 1);
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
	player1 = new Player(*assetManager.get_model(0), startingPosition + glm::vec3(0, 6, 0), glm::vec3(2.0f));
	player2 = new Player(*assetManager.get_model(0), startingPosition + glm::vec3(0, 6, 0), glm::vec3(2.0f));
	player3 = new Player(*assetManager.get_model(0), startingPosition + glm::vec3(0, 6, 0), glm::vec3(2.0f));
	player4 = new Player(*assetManager.get_model(0), startingPosition + glm::vec3(0, 6, 0), glm::vec3(2.0f));
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