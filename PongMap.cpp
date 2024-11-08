#include"PongMap.h"

int i = 1;
glm::vec3 direction(0.0f, 0.0f, 0.5f);

PongMap::PongMap(std::string mapName, DebugMenu debugMenu) : Map(mapName, debugMenu)
{
	camera.setCameraPos(glm::vec3(-100, 100, 0));
}

glm::vec3 local_to_world(const glm::vec3& localPos, const glm::vec3& position, const glm::vec3& scale)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f); 

	modelMatrix = glm::translate(modelMatrix, position); 
	modelMatrix = glm::scale(modelMatrix, scale);

	glm::vec4 worldPos = modelMatrix * glm::vec4(localPos, 1.0f);

	return glm::vec3(worldPos);
}


void PongMap::update()
{
	if (debugMenu.is_menu_open() == false)
	{
		camera.setCameraPos(glm::vec3(-100, 100, 0));
		camera.setCameraFront(glm::normalize(glm::vec3(0.0f) - camera.getCameraPos())); 
		camera.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
	}
	
	if (abs(this->entities.at(ballVal).Position.z) > 50.0f)
	{
		direction.z *= -1;
	}
	if (abs(this->entities.at(ballVal).Position.x) > 50.0f)
	{
		direction.x *= -1;
	}


	player_collision(player1);
	player_collision(player2);
	player_collision(player3);
	player_collision(player4);


	this->entities.at(ballVal).Position += direction;
}

void PongMap::load_players(AssetManager assetManager)
{
	player1 = new Player(*assetManager.get_model(0), glm::vec3(0, 3, 20), glm::vec3(2.0f));
	player2 = new Player(*assetManager.get_model(0), glm::vec3(15, 3, 40), glm::vec3(2.0f));
	player3 = new Player(*assetManager.get_model(0), glm::vec3(0, 3, -20), glm::vec3(2.0f));
	player4 = new Player(*assetManager.get_model(0), glm::vec3(15, 3, -40), glm::vec3(2.0f));
}

void PongMap::process_input(InputManager& inputManager, float deltaTime)
{
	menu_input(inputManager, deltaTime);

	if (inputManager.Keys[GLFW_KEY_UP])
		player1->velocity.x = 100.0; 
	else if (inputManager.Keys[GLFW_KEY_DOWN])
		player1->velocity.x = -100.0;
	else
		player1->velocity.x = 0; 

	if (inputManager.Keys[GLFW_KEY_F2])
		player2->velocity.x = 100.0; 
	else if (inputManager.Keys[GLFW_KEY_F3])
		player2->velocity.x = -100.0;
	else
		player2->velocity.x = 0;  
	if (inputManager.Keys[GLFW_KEY_LEFT])
		player3->velocity.x = -100.0; 
	else if (inputManager.Keys[GLFW_KEY_RIGHT])
		player3->velocity.x = 100.0;
	else
		player3->velocity.x = 0; 

	if (inputManager.Keys[GLFW_KEY_F4])
		player4->velocity.x = 100.0;
	else if (inputManager.Keys[GLFW_KEY_F5])
		player4->velocity.x = -100.0;
	else
		player4->velocity.x = 0; 

	//move to update eventually
	player1->Position.x += player1->velocity.x * deltaTime;
	player2->Position.x += player2->velocity.x * deltaTime;
	player3->Position.x += player3->velocity.x * deltaTime;
	player4->Position.x += player4->velocity.x * deltaTime;
}

void PongMap::player_collision(Player* player)
{
	// Get the world space min and max AABBs for player1
	glm::vec3 player1MinAABB = local_to_world(player->GameModel.getMinAABB(), player->Position, glm::vec3(2.0f));
	glm::vec3 player1MaxAABB = local_to_world(player->GameModel.getMaxAABB(), player->Position, glm::vec3(2.0f));


	// Check if the ball's position is within the world space AABB of player1
	if (this->entities.at(ballVal).Position.x >= player1MinAABB.x - 5.0f && this->entities.at(ballVal).Position.x <= player1MaxAABB.x + 3.0f
		&& this->entities.at(ballVal).Position.z >= player1MinAABB.z && this->entities.at(ballVal).Position.z <= player1MaxAABB.z)
	{
		std::cout << "Collision detected!" << "\n";
		i *= -1;
		std::cout << "------------dir" << "\n";
		std::cout << direction.x << "\n";
		std::cout << direction.z << "\n";
		std::cout << "------------dir" << "\n";
		std::cout << player->velocity.x << "\n";
		std::cout << player->velocity.z << "\n";
		direction *= -1;
		direction -= player->velocity * glm::vec3(0.002f);
	}
}