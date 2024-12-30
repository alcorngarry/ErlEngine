#include"Player.h"


Player::Player(Model* characterModel, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation) : GameObject(0, characterModel, pos, size, rotation)
{ 
	mation = new Animation((char*)"C:/Dev/assets/knight/knight_skinned_0/knight_impl1.dae", characterModel);
	mator = new Animator(mation);
	boardPosition = 0;
	inMotion = false;
	transforms = mator->get_final_bone_matrices();
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
			glm::vec3 objectPos = spaces[boardPosition];
			glm::vec3 direction = (spaces[nextPos] - spaces[boardPosition]);
			glm::mat4 test = glm::mat4(1.0f);
			glm::vec3 currPos = objectPos + direction * ((float)glfwGetTime() - startTime) / delta;

			Position = currPos;
		}
		else {
			boardPosition = nextPos;
			startTime = (float)glfwGetTime();

			Position = spaces[boardPosition];
		}
	}
	else {
		Position = spaces[boardPosition];
		mator->reset_animation();
		transforms = mator->get_final_bone_matrices();
		inMotion = false;
	}
}

void Player::update(float deltaTime)
{
	transforms = mator->get_final_bone_matrices();
	mator->update_animation(deltaTime);
}

//void Player::play_animation()
//{
//	mator->play_animation(mation);
//}

unsigned int Player::get_board_position()
{
	return boardPosition;
}

void Player::start_move(float startTime, int moves)
{
	this->startTime = startTime;
	this->moves = boardPosition + moves;
}