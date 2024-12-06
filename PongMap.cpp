#include"PongMap.h"

glm::vec3 direction(0.0f, 0.0f, 0.25f);

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
	
	if (abs(this->entities.back().Position.z) > 50.0f)
	{
		if (direction.z < 0) {
			team_1_points++;
			std::cout << "Team 1's score: " << team_1_points << "\n";
		}
		else {
			team_2_points++;
			std::cout << "Team 2's score: " << team_2_points << "\n";
		};

		this->entities.back().Position = glm::vec3(0.0f, 5.0f,0.0f);
		direction.z *= -1;
		direction.x = 0.0f;
	}
	if (abs(this->entities.back().Position.x) > 50.0f)
	{
		direction.x *= -1;
	}

	players.at(0).update(deltaTime);
	players.at(1).update(deltaTime);
	players.at(2).update(deltaTime);
	players.at(3).update(deltaTime);

	players.at(0).Position.x += players.at(0).velocity.x * deltaTime;
	players.at(1).Position.x += players.at(1).velocity.x * deltaTime;
	players.at(2).Position.x += players.at(2).velocity.x * deltaTime;
	players.at(3).Position.x += players.at(3).velocity.x * deltaTime;

	player_collision(&players.at(0));
	player_collision(&players.at(1));
	player_collision(&players.at(2));
	player_collision(&players.at(3));

	//need to figure new way to store objects, probably will transfer to xml......
	this->entities.back().Position += direction;
}

void PongMap::load_players(AssetManager assetManager)
{
	players.push_back(Player(*assetManager.get_model(0), glm::vec3(0, 4, 20), glm::vec3(2.0f)));
	players.push_back(Player(*assetManager.get_model(0), glm::vec3(15, 4, 40), glm::vec3(2.0f)));
	players.push_back(Player(*assetManager.get_model(0), glm::vec3(0, 4, -20), glm::vec3(2.0f)));
	players.push_back(Player(*assetManager.get_model(0), glm::vec3(15, 4, -40), glm::vec3(2.0f)));

	this->entities.push_back(GameObject(5, *assetManager.get_model(5), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2, 3, 3), glm::vec3(0.0f, 0.0f, 0.0f)));
}

glm::vec3 local_to_world(const glm::vec3& localPos, const glm::vec3& position, const glm::vec3& scale)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scale);

	glm::vec4 worldPos = modelMatrix * glm::vec4(localPos, 1.0f);

	return glm::vec3(worldPos);
}

void PongMap::process_input(InputManager& inputManager, float deltaTime)
{
	menu_input(inputManager, deltaTime);

	if (inputManager.Keys[GLFW_KEY_UP])
		players.at(0).velocity.x = 100.0;
	else if (inputManager.Keys[GLFW_KEY_DOWN])
		players.at(0).velocity.x = -100.0;
	else
		players.at(0).velocity.x = 0; 
	if (inputManager.Keys[GLFW_KEY_F2])
		players.at(1).velocity.x = 100.0; 
	else if (inputManager.Keys[GLFW_KEY_F3])
		players.at(1).velocity.x = -100.0;
	else
		players.at(1).velocity.x = 0;  
	if (inputManager.Keys[GLFW_KEY_LEFT])
		players.at(2).velocity.x = -100.0; 
	else if (inputManager.Keys[GLFW_KEY_RIGHT])
		players.at(2).velocity.x = 100.0;
	else
		players.at(2).velocity.x = 0; 
	if (inputManager.Keys[GLFW_KEY_F4])
		players.at(3).velocity.x = 100.0;
	else if (inputManager.Keys[GLFW_KEY_F5])
		players.at(3).velocity.x = -100.0;
	else
		players.at(3).velocity.x = 0;
}

void PongMap::player_collision(Player* player)
{
	glm::vec3 player1MinAABB = local_to_world(player->GameModel.getMinAABB(), player->Position, player->Size);
	glm::vec3 player1MaxAABB = local_to_world(player->GameModel.getMaxAABB(), player->Position, player->Size);

	if (this->entities.back().Position.x >= player1MinAABB.x - 5.0f && this->entities.back().Position.x <= player1MaxAABB.x + 3.0f
		&& this->entities.back().Position.z >= player1MinAABB.z && this->entities.back().Position.z <= player1MaxAABB.z)
	{
	/*	std::cout << "Collision detected!" << "\n";
		std::cout << "------------dir" << "\n";
		std::cout << direction.x << "\n";
		std::cout << direction.z << "\n";
		std::cout << "------------dir" << "\n";
		std::cout << player->velocity.x << "\n";
		std::cout << player->velocity.z << "\n";*/
		direction *= -1;
		direction -= player->velocity * glm::vec3(0.002f);
	}
}