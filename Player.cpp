#include"Player.h"

Player::Player(Model characterModel, glm::vec3 pos, glm::vec3 size) : GameObject(characterModel, pos, size)
{ 
	this->boardPosition = 0;
	this->inMotion = false;
}

void Player::move_player(std::vector<glm::vec3> spaces)
{
	if (moves >= spaces.size()) moves -= spaces.size();

	int nextPos = boardPosition == spaces.size() - 1 ? 0 : boardPosition + 1;
	float delta = 1.0f;

	if (boardPosition != moves)
	{
		if ((float)glfwGetTime() < startTime + delta)
		{
			//algorithm to travel distance given start and delta time.
			//A + (B -A)(t - t0)/deltaT
			glm::vec3 objectPos = spaces[boardPosition] * 5.0f;
			glm::vec3 direction = (spaces[nextPos] - spaces[boardPosition]) * 5.0f;
			glm::mat4 test = glm::mat4(1.0f);
			glm::vec3 currPos = objectPos + direction * ((float)glfwGetTime() - startTime) / delta + glm::vec3(0, 3, 0);

			//	camera.setCameraPos(currPos + glm::vec3(0.0f, 30.0f, 50.0f));
			//	camera.setCameraFront(currPos);

			this->Position = currPos;
		}
		else {
			boardPosition = nextPos;
			startTime = (float)glfwGetTime();

			this->Position = spaces[boardPosition] * 5.0f + glm::vec3(0, 3, 0);
		}
	}
	else {
		this->Position =  spaces[boardPosition] * 5.0f + glm::vec3(0, 3, 0);
		this->inMotion = false;

		/*	camera.setCameraPos(objectsInScene[startPos] * 10.0f + glm::vec3(0, 3, 0) + glm::vec3(0.0f, 30.0f, 50.0f));
			camera.setCameraFront(objectsInScene[startPos] * 10.0f + glm::vec3(0, 3, 0));*/

	}
}

void Player::start_move(float startTime, int moves)
{
	this->startTime = startTime;
	this->moves = boardPosition + moves;
}