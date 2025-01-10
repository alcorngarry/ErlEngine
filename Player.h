#ifndef PLAYER_H
#define PLAYER_H
#define GLFW_INCLUDE_NONE
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

#include"Animator.h"
#include"GameObject.h"

class InputManager;

struct PlayerControls {
	//pong map atm
	unsigned int move_up;
	unsigned int move_down;
	//board turn controls
	unsigned int select_card;
	unsigned int left;
	unsigned int right;
};

class Player : public GameObject {
	public:
		bool inMotion = false;
		int moves = 0;
		glm::vec3 velocity = glm::vec3(0.0f);
		std::vector<glm::mat4>* transforms;
		Animator* mator;
		Animation* mation;
		bool isTurn = false;
		unsigned int groats = 0;
		unsigned int selectedCardIndex = 0;

		Player(unsigned int playerId, Model* characterModel, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, PlayerControls controls);
		void move_player(std::vector<GameObject*> boardSpaces);
		void start_move(float startTime, int moves);
		unsigned int get_board_position();
		void update(float deltaTime) override;
		void process_player_input(InputManager* inputManager, float deltaTime);
		void init_deck();
		unsigned int* get_cards();
		unsigned int get_selected_card_index();
		bool is_turn();
		void toggle_turn();
		void add_groats(unsigned int groats);
		void remove_groats(unsigned int groats);

	private:
		unsigned int draw_card();
		int boardPosition = 0;
		float startTime;
		unsigned int playerId = -1;
		PlayerControls controls;
		unsigned int cards[5];
		

		float roll_dice();
		PlayerControls set_controls(unsigned int id);
};
#endif // !PLAYER_H
