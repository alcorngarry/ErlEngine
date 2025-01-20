#include"BoardMap.h"

BoardMap::BoardMap(std::string mapName) : Map(mapName)
{
	//think about it, set states for player turns.
	//each player has a deck, implement deck logic -- mostly complete, needs more cards, and finite amount...maybe
	//add types of spaces -- in progress
	//add controller controls
	//add item select system -- in progress
	//need ui for items -- in progress

	// part deux
	// fix camera follow system -- in progress
	// fix player orientation when traversing the map --fixed
	// add groat ui elements and text
	// add esc command end program --done
	// add save command, if it doesn't already exist --save needs tweaking. the save file gets corrupt.
}

void BoardMap::update(float deltaTime)
{
	set_controls(deltaTime);

	// Update the camera's position
	// tan = a/h, h is what we want.
	// a over tan gives u h
	if (state == DEFAULT)
	{
		if (players[currentPlayer]->inMotion)
		{
			float playerYRotation = glm::radians(players[currentPlayer]->Rotation.y);
			glm::vec3 playerDirection = glm::vec3(sin(playerYRotation), 0.0f, cos(playerYRotation));
			float distanceBehind = 80.0f;
			float heightAbove = 30.0f;
			glm::vec3 cameraOffset = (playerDirection * -distanceBehind) + glm::vec3(0.0f, heightAbove, 0.0f);

			camera->setCameraPos(players[currentPlayer]->Position + cameraOffset);
			players[currentPlayer]->update(deltaTime);
			players[currentPlayer]->move_player(get_board_objects());
			players[currentPlayer]->state = INACTIVE;
		}
		else {
			//camera->setCameraPos(players[currentPlayer]->Position + glm::vec3(-80.0f, 30.0f, 0.0f));
			glm::vec3 direction = players[currentPlayer]->Position - camera->getCameraPos();
			players[currentPlayer]->Rotation = -1.0f * glm::vec3(0.0f, glm::degrees(std::atan2(direction.x, direction.z)), 0.0f);
		}

		camera->setCameraFront(players[currentPlayer]->Position - camera->getCameraPos());

		if (players[currentPlayer]->state == INACTIVE && !players[currentPlayer]->inMotion)
		{
			process_board_space(get_board_objects()[players[currentPlayer]->get_board_position()]->id);
			currentPlayer = currentPlayer == players.size() - 1 ? 0 : currentPlayer += 1;
			players[currentPlayer]->state = ACTIVE;
		}
	}
}

void BoardMap::draw(float deltaTime)
{
	Renderer::render(players, entities, lights, camera, skybox);
}

//remove delta time after fixing animation call structure
void BoardMap::set_controls(float deltaTime)
{
	for (Player* player : players)
	{
		if (player->state == ACTIVE)
		{
			//rework gamepad binding to bind player to gamepad.
			InputManager::set_gamepad_binding(GLFW_GAMEPAD_BUTTON_DPAD_UP, new MoveUpCommand(players[currentPlayer], deltaTime));
			InputManager::set_gamepad_binding(GLFW_GAMEPAD_BUTTON_DPAD_DOWN, new MoveDownCommand(players[currentPlayer], deltaTime));
			InputManager::set_gamepad_binding(GLFW_GAMEPAD_BUTTON_DPAD_LEFT, new SelectCardLeftCommand(players[currentPlayer]));
			InputManager::set_gamepad_binding(GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, new SelectCardRightCommand(players[currentPlayer]));
			InputManager::set_gamepad_binding(GLFW_GAMEPAD_BUTTON_A, new SelectCardCommand(players[currentPlayer]));
		}
	}
}

void BoardMap::load_players()
{
	//add height buffer for character
	glm::vec3 startingPosition = get_board_objects()[0]->Position + glm::vec3(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < 1; i++)
	{
		players.push_back(new Player(i, AssetManager::get_model(0), startingPosition, glm::vec3(2.0f), glm::vec3(0.0f), playerControls));
		players[i]->init_deck();

		if (i == currentPlayer)
		{
			players[i]->state = ACTIVE;
		}
	}
}

void BoardMap::process_board_space(unsigned int boardId)
{
	switch (boardId)
	{
		case 0: 
			//loadState = CHANGE_MAP;
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