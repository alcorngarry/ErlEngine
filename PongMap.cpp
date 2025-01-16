#include"PongMap.h"

glm::vec3 ball_velocity(0.0f, 0.0f, 40.0f);
int ballIndex = -1;

PongMap::PongMap(std::string mapName) : Map(mapName)
{
	camera->setCameraPos(glm::vec3(-100, 100, 0));
}

void PongMap::update(float deltaTime)
{
	//if (state == MENU_CLOSED)
	//{
		camera->setCameraPos(glm::vec3(-100, 100, 0));
		camera->setCameraFront(glm::normalize(glm::vec3(0.0f) - camera->getCameraPos())); 
		camera->setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
	//}
	
	if (abs(entities[ballIndex]->Position.z) > 50.0f)
	{
		if (ball_velocity.z < 0) {
			team_1_points++;
			std::cout << "Team 1's score: " << team_1_points << "\n";
		}
		else {
			team_2_points++;
			std::cout << "Team 2's score: " << team_2_points << "\n";
		};

		entities[ballIndex]->Position = glm::vec3(0.0f, 5.0f,0.0f);
		ball_velocity.z *= -1;
		ball_velocity.x = 0.0f;
	}
	if (abs(entities[entities.size() - 1]->Position.x) > 50.0f)
	{
		ball_velocity.x *= -1;
	}

	players[0]->Position.x += players[0]->velocity.x * deltaTime;
	players[1]->Position.x += players[1]->velocity.x * deltaTime;
	players[2]->Position.x += players[2]->velocity.x * deltaTime;
	players[3]->Position.x += players[3]->velocity.x * deltaTime;

	check_ball_collision(entities[ballIndex]);

	//need to figure new way to store objects, probably will transfer to xml......
	entities[ballIndex]->Position = glm::vec3(ball_velocity * deltaTime);
}

void PongMap::load_players()
{
	players.push_back(new Player(0, AssetManager::get_model(0), glm::vec3(0, 4, 20), glm::vec3(2.0f), glm::vec3(0.0f, 180.0f, 0.0f), playerControls));
	players.push_back(new Player(1, AssetManager::get_model(0), glm::vec3(15, 4, 40), glm::vec3(2.0f), glm::vec3(0.0f, 180.0f, 0.0f), playerControls));
	players.push_back(new Player(2, AssetManager::get_model(0), glm::vec3(0, 4, -20), glm::vec3(2.0f), glm::vec3(0.0f, 0.0f, 0.0f), playerControls));
	players.push_back(new Player(3, AssetManager::get_model(0), glm::vec3(15, 4, -40), glm::vec3(2.0f), glm::vec3(0.0f, 0.0f, 0.0f), playerControls));

	entities.push_back(new GameObject(5, AssetManager::get_model(5), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(2, 3, 3), glm::vec3(0.0f)));
	ballIndex = entities.size() - 1;
}

void PongMap::set_controls(float deltaTime)
{
}

void PongMap::check_ball_collision(GameObject* entity)
{
	for (Player* player : players)
	{
		if (entity->Position.x >= player->get_aabb_min().x && entity->Position.x <= player->get_aabb_max().x
			&& entity->Position.z >= player->get_aabb_min().z && entity->Position.z <= player->get_aabb_max().z)
		{
			ball_velocity *= -1;
			ball_velocity += player->velocity;
		}
	}
}