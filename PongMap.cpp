#include"PongMap.h"

glm::vec3 ball_velocity(0.0f, 0.0f, 40.0f);

PongMap::PongMap(std::string mapName) : Map(mapName)
{
	camera.setCameraPos(glm::vec3(-100, 100, 0));
}

void PongMap::update(float deltaTime)
{
	if (State == MENU_CLOSED)
	{
		camera.setCameraPos(glm::vec3(-100, 100, 0));
		camera.setCameraFront(glm::normalize(glm::vec3(0.0f) - camera.getCameraPos())); 
		camera.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
	}
	
	if (abs(entities[entities.size() - 1]->Position.z) > 50.0f)
	{
		if (ball_velocity.z < 0) {
			team_1_points++;
			std::cout << "Team 1's score: " << team_1_points << "\n";
		}
		else {
			team_2_points++;
			std::cout << "Team 2's score: " << team_2_points << "\n";
		};

		entities[entities.size() - 1]->Position = glm::vec3(0.0f, 5.0f,0.0f);
		ball_velocity.z *= -1;
		ball_velocity.x = 0.0f;
	}
	if (abs(entities[entities.size() - 1]->Position.x) > 50.0f)
	{
		ball_velocity.x *= -1;
	}

	//players[0]->update(deltaTime);
	//players[1]->update(deltaTime);
	//players[2]->update(deltaTime);
	//players[3]->update(deltaTime);

	players[0]->Position.x += players[0]->velocity.x * deltaTime;
	players[1]->Position.x += players[1]->velocity.x * deltaTime;
	players[2]->Position.x += players[2]->velocity.x * deltaTime;
	players[3]->Position.x += players[3]->velocity.x * deltaTime;

	check_ball_collision(entities[entities.size() - 1]);


	//need to figure new way to store objects, probably will transfer to xml......
	entities[entities.size() - 1]->Position += ball_velocity * deltaTime;
}

void PongMap::load_players(AssetManager assetManager)
{
	players.push_back(new Player(assetManager.get_model(0), glm::vec3(0, 4, 20), glm::vec3(0.08f), glm::vec3(0.0f, 180.0f, 0.0f)));
	players.push_back(new Player(assetManager.get_model(0), glm::vec3(15, 4, 40), glm::vec3(0.08f), glm::vec3(0.0f, 180.0f, 0.0f)));
	players.push_back(new Player(assetManager.get_model(0), glm::vec3(0, 4, -20), glm::vec3(0.08f), glm::vec3(0.0f)));
	players.push_back(new Player(assetManager.get_model(0), glm::vec3(15, 4, -40), glm::vec3(0.08f), glm::vec3(0.0f)));

	entities.push_back(new GameObject(5, assetManager.get_model(5), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2, 3, 3), glm::vec3(0.0f)));
}

void PongMap::process_input(InputManager& inputManager, float deltaTime)
{
	menu_input(inputManager, deltaTime);

	if (inputManager.Keys[GLFW_KEY_UP])
	{
		players[0]->velocity.x = 100.0;
		players[0]->update(deltaTime);
	}
	else if (inputManager.Keys[GLFW_KEY_DOWN])
	{
		players[0]->velocity.x = -100.0;
		players[0]->update(deltaTime);
	}
	else
		players[0]->velocity.x = 0;
	if (inputManager.Keys[GLFW_KEY_F2])
	{
		players[1]->velocity.x = 100.0;
		players[1]->update(deltaTime);
	}
	else if (inputManager.Keys[GLFW_KEY_F3])
	{
		players[1]->velocity.x = -100.0;
		players[1]->update(deltaTime);
	}
	else
		players[1]->velocity.x = 0;
	if (inputManager.Keys[GLFW_KEY_LEFT])
	{
		players[2]->velocity.x = -100.0;
		players[2]->update(deltaTime);
	}
	else if (inputManager.Keys[GLFW_KEY_RIGHT])
	{
		players[2]->velocity.x = 100.0;
		players[2]->update(deltaTime);
	}
	else
		players[2]->velocity.x = 0;
	if (inputManager.Keys[GLFW_KEY_F4])
	{
		players[3]->velocity.x = 100.0;
		players[3]->update(deltaTime);
	}
	else if (inputManager.Keys[GLFW_KEY_F5])
	{
		players[3]->velocity.x = -100.0;
		players[3]->update(deltaTime);
	}
	else
		players[3]->velocity.x = 0;
}

glm::vec3 local_to_world(glm::vec3 localPos, const glm::vec3& position, const glm::vec3& scale)
{
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
	modelMatrix = glm::scale(modelMatrix, scale);
	glm::vec4 worldPos = modelMatrix * glm::vec4(localPos, 1.0f);

	return glm::vec3(worldPos);
}

void PongMap::check_ball_collision(GameObject* entity)
{
	for (Player* player : players)
	{
		// Get the world space min and max AABBs for player1
		glm::vec3 playerMinAABB = local_to_world(player->GameModel->getMinAABB(), player->Position, glm::vec3(0.08f));
		glm::vec3 playerMaxAABB = local_to_world(player->GameModel->getMaxAABB(), player->Position, glm::vec3(0.08f));

		if (entity->Position.x >= playerMinAABB.x && entity->Position.x <= playerMaxAABB.x
			&& entity->Position.z >= playerMinAABB.z && entity->Position.z <= playerMaxAABB.z)
		{
			ball_velocity *= -1;
			ball_velocity += player->velocity;
		}
	}
}