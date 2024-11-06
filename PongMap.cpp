#include"PongMap.h"

int i = 1;

PongMap::PongMap(std::string mapName, DebugMenu debugMenu) : Map(mapName, debugMenu)
{
	camera.setCameraPos(glm::vec3(-50, 90, 0));
}

glm::vec3 local_to_world(const glm::vec3& localPos, const glm::vec3& position, const glm::vec3& scale)
{
	// Create the model matrix (you might have this already as part of Player's transformation)
	glm::mat4 modelMatrix = glm::mat4(1.0f);  // Identity matrix by default

	// Apply the scale, rotation, and translation
	modelMatrix = glm::translate(modelMatrix, position); // Apply translation
	modelMatrix = glm::scale(modelMatrix, scale); // Apply scaling

	// Convert the local position to world space by multiplying with the model matrix
	glm::vec4 worldPos = modelMatrix * glm::vec4(localPos, 1.0f); // Homogeneous coordinate (w = 1.0)

	// Return the resulting world position (convert back to 3D point)
	return glm::vec3(worldPos);
}


void PongMap::update()
{
	if (debugMenu.is_menu_open() == false)
	{
		camera.setCameraPos(glm::vec3(-50, 90, 0));
		camera.setCameraFront(glm::normalize(glm::vec3(0.0f) - camera.getCameraPos())); 
		camera.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
	}
	
	if (abs(this->entities.at(ballVal).Position.z) > 50.0f)
	{
		i *= -1;
	}

	player_collision(player1);
	player_collision(player2);
	player_collision(player3);
	player_collision(player4);

	this->entities.at(ballVal).Position.z += (0.5f * i);
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

	if(inputManager.Keys[GLFW_KEY_UP])
	{
		player1->Position.x += .5;
	}
	if(inputManager.Keys[GLFW_KEY_DOWN])
	{
		player1->Position.x += -.5;
	}
	if (inputManager.Keys[GLFW_KEY_F2])
	{
		player2->Position.x += .5;
	}
	if (inputManager.Keys[GLFW_KEY_F3])
	{
		player2->Position.x += -.5;
	}
	if(inputManager.Keys[GLFW_KEY_LEFT])
	{
		player3->Position.x += -.5;
	}
	if(inputManager.Keys[GLFW_KEY_RIGHT])
	{
		player3->Position.x += .5;
	}
	if (inputManager.Keys[GLFW_KEY_F4])
	{
		player4->Position.x += .5;
	}
	if (inputManager.Keys[GLFW_KEY_F5])
	{
		player4->Position.x += -.5;
	}
}

void PongMap::player_collision(Player* player)
{
	// Get the world space min and max AABBs for player1
	glm::vec3 player1MinAABB = local_to_world(player->GameModel.getMinAABB(), player->Position, glm::vec3(2.0f));
	glm::vec3 player1MaxAABB = local_to_world(player->GameModel.getMaxAABB(), player->Position, glm::vec3(2.0f));


	// Check if the ball's position is within the world space AABB of player1
	if (this->entities.at(ballVal).Position.x >= player1MinAABB.x - 4.0f && this->entities.at(ballVal).Position.x <= player1MaxAABB.x + 3.0f
		&& this->entities.at(ballVal).Position.z >= player1MinAABB.z && this->entities.at(ballVal).Position.z <= player1MaxAABB.z)
	{
		std::cout << "Collision detected!" << "\n";
		i *= -1;  // Reverse ball direction
	}
}