#include"Player.h"
#include"InputManager.h"

Player::Player(unsigned int playerId, Model* characterModel, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, PlayerControls controls) : GameObject(0, characterModel, pos, size, rotation)
{ 
	this->playerId = playerId;
	mation = new Animation((char*)"C:/Dev/assets/knight/knight_skinned_0/knight_impl1.dae", characterModel);
	mator = new Animator(mation);
	transforms = mator->get_final_bone_matrices();
	this->controls = controls;
}

void Player::move_player(std::vector<GameObject*> boardSpaces)
{		
	int nextPos = boardPosition == boardSpaces.size() - 1 ? 0 : boardPosition + 1;
	float delta = 1.0f;

  	if (boardPosition != moves)
	{
		if ((float)glfwGetTime() < startTime + delta)
		{
			//algorithm to travel distance given start and delta time.
			//A + (B -A)(t - t0)/deltaT
			glm::vec3 objectPos = boardSpaces[boardPosition]->Position;
			glm::vec3 direction = (boardSpaces[nextPos]->Position - boardSpaces[boardPosition]->Position);
			glm::mat4 test = glm::mat4(1.0f);
			glm::vec3 currPos = objectPos + direction * ((float)glfwGetTime() - startTime) / delta;

			Position = currPos;
		}
		else {
			boardPosition = nextPos;
			startTime = (float)glfwGetTime();

			//figure rotation lol
			Position = boardSpaces[boardPosition]->Position;
		}
	}
	else {
		Position = boardSpaces[boardPosition]->Position;
		mator->reset_animation();
		transforms = mator->get_final_bone_matrices();
		inMotion = false;
		toggle_turn();

		cards[selectedCardIndex] = draw_card();
		selectedCardIndex = 0; //reset to start on card 1
	}

	//fix this
	if (moves >= boardSpaces.size())
	{
		moves -= boardSpaces.size();
		//do an actual implementation;
	}
}

void Player::update(float deltaTime)
{
	transforms = mator->get_final_bone_matrices();
	mator->update_animation(deltaTime);
}

void Player::process_player_input(InputManager* inputManager, float deltaTime)
{
	if (inputManager->Keys[controls.select_card] && !inputManager->KeysProcessed[controls.select_card])
	{
		if (inMotion == false)
		{
			inMotion = true;
			start_move((float)glfwGetTime(), cards[selectedCardIndex]);
		}
		inputManager->KeysProcessed[controls.select_card] = true;
	}
	if (inputManager->Keys[controls.move_up])
	{
		velocity.x = 100.0;
		update(deltaTime);
	}
	else if (inputManager->Keys[controls.move_down])
	{
		velocity.x = -100.0;
		update(deltaTime);
	}
	else {
		velocity.x = 0;
	}
	// card select
	if (inputManager->Keys[controls.left] && !inputManager->KeysProcessed[controls.left])
	{
		selectedCardIndex == 0 ? 0 : selectedCardIndex--;
		std::cout << "Current card selected " << selectedCardIndex << std::endl;
		inputManager->KeysProcessed[controls.left] = true;
	}

	if (inputManager->Keys[controls.right] && !inputManager->KeysProcessed[controls.right])
	{
		selectedCardIndex == 4 ? 4 : selectedCardIndex++;
		std::cout << "Current card selected " << selectedCardIndex << std::endl;
		inputManager->KeysProcessed[controls.right] = true;
	}
}

unsigned int Player::get_board_position()
{
	return boardPosition;
}

void Player::start_move(float startTime, int moves)
{
	this->startTime = startTime;
	// can set total moves if needed here.
	this->moves = boardPosition + moves;
}

//float Player::roll_dice()
//{
//	srand(time(0));
//	int roll = rand() % 6;
//
//	std::cout << "rolled " << roll + 1 << std::endl;
//	return roll + 1;
//}

void Player::init_deck()
{
	for (int i = 0; i < 5; i++)
	{
		cards[i] = draw_card();
		std::cout << "card " << i << ": " << cards[i] << std::endl;
	}
}

unsigned int Player::draw_card()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 13);

	return dis(gen);
}

unsigned int* Player::get_cards()
{
	return cards;
}

unsigned int Player::get_selected_card_index()
{
	return selectedCardIndex;
}

void Player::add_groats(unsigned int groats)
{
	this->groats += groats;
}

void Player::remove_groats(unsigned int groats)
{
	if (this->groats >= groats)
	{
		this->groats -= groats;
	}
}

bool Player::is_turn()
{
	return isTurn;
}

void Player::toggle_turn()
{
	isTurn = !isTurn;
}