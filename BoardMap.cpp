#include"BoardMap.h"

bool turnOver = false;
//PlayerControls playerControls{ GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_SPACE, GLFW_KEY_LEFT, GLFW_KEY_RIGHT };

BoardMap::BoardMap(std::string mapName) : Map(mapName)
{
	//think about it, set states for player turns.
	//each player has a deck, implement deck logic -- mostly complete, needs more cards, and finite amount...maybe
	//add types of spaces -- in progress
	//add controller controls
	//add item select system -- in progress
	//need ui for items -- in progress
}

void BoardMap::update(float deltaTime)
{
	if (state == MENU_CLOSED)
	{
		camera.setCameraPos(players[currentPlayer]->Position + glm::vec3(-80.0f, 30.0f, 0.0f));
		camera.setCameraFront(players[currentPlayer]->Position - camera.getCameraPos());
	}
	// move this to player update method !!!
	if (players[currentPlayer]->inMotion)
	{
		players[currentPlayer]->update(deltaTime);
		players[currentPlayer]->move_player(get_board_objects());
		turnOver = true;
	}
	if (turnOver == true && !players[currentPlayer]->inMotion)
	{
		turnOver = false;
		//players[currentPlayer]->toggle_turn();
		players[currentPlayer]->isTurn = false;

		process_board_space(get_board_objects()[players[currentPlayer]->get_board_position()]->id);

		currentPlayer = currentPlayer == players.size() - 1 ? 0 : currentPlayer += 1;

		players[currentPlayer]->isTurn = true;

	}
}

void BoardMap::draw(float deltaTime)
{
	Renderer::render(players, entities, lights, camera, skybox);

	if (state == MENU_OPEN)
	{
		Renderer::create_menu(deltaTime, players[0]->moves);
	}
}

void BoardMap::process_input(InputManager* inputManager, float deltaTime)
{
	inputManager->set_key_binding(GLFW_KEY_UP, new MoveUpCommand(players[currentPlayer], deltaTime));
	inputManager->set_key_binding(GLFW_KEY_DOWN, new MoveDownCommand(players[currentPlayer], deltaTime));
	inputManager->set_key_binding(GLFW_KEY_LEFT, new SelectCardLeftCommand(players[currentPlayer], deltaTime));
	inputManager->set_key_binding(GLFW_KEY_RIGHT, new SelectCardRightCommand(players[currentPlayer], deltaTime));
	inputManager->set_key_binding(GLFW_KEY_SPACE, new SelectCardCommand(players[currentPlayer], deltaTime));
		/*keyBindings[GLFW_KEY_UP] = new MoveUpCommand();
		keyBindings[GLFW_KEY_DOWN] = new MoveDownCommand();
		keyBindings[GLFW_KEY_LEFT] = new SelectCardLeftCommand();
		keyBindings[GLFW_KEY_RIGHT] = new SelectCardRightCommand();
		keyBindings[GLFW_KEY_SPACE] = new SelectCardCommand();*/
	inputManager->update();
	menu_input(inputManager, deltaTime);

	players[currentPlayer]->process_player_input(inputManager, deltaTime);
}

void BoardMap::load_players()
{
	//add height buffer for character
	glm::vec3 startingPosition = get_board_objects()[0]->Position + glm::vec3(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < 4; i++)
	{
		players.push_back(new Player(i, AssetManager::get_model(0), startingPosition, glm::vec3(2.0f), glm::vec3(0.0f), playerControls));
		players[i]->init_deck();

		if (i == currentPlayer)
		{
			players[i]->toggle_turn();
		}
	}
}

void BoardMap::process_board_space(unsigned int boardId)
{
	switch (boardId)
	{
		case 0: 
			loadState = CHANGE_MAP;
			break;
		case 1:
			players[currentPlayer]->add_groats(3);
			std::cout << "added 3 groats to player: " << currentPlayer << ", total groats = " << players[currentPlayer]->groats << std::endl;
			break;
		case 2:
			players[currentPlayer]->add_groats(6);
			std::cout << "added 6 groats to player: " << currentPlayer << ", total groats = " << players[currentPlayer]->groats << std::endl;
			break;
		case 3:
			players[currentPlayer]->remove_groats(3);
			std::cout << "removed 3 groats to player: " << currentPlayer << ", total groats = " << players[currentPlayer]->groats << std::endl;
			break;
		default: 
			std::cout << "error processing space" << std::endl;
	}
}

//todo add indexes to auto fix order of paths
std::vector<GameObject*> BoardMap::get_board_objects()
{
	std::vector<GameObject*> output;

	for (GameObject* entity : entities)
	{
		if (entity->id == 0 || entity->id == 1 || entity->id == 2 || entity->id == 3)
		{
			output.push_back(entity);
		}
	}
	return output;
}