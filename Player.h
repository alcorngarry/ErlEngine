#pragma once
#ifndef PLAYER_H
#define GLFW_INCLUDE_NONE
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include"Animator.h"
#include"GameObject.h"

class Player : public GameObject {
	public:
		bool inMotion;
		glm::vec3 velocity = glm::vec3(0.0f);
		std::vector<glm::mat4>* transforms;

		Player(Model* characterModel, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
		void move_player(std::vector<glm::vec3> spaces);
		void start_move(float startTime, int moves);
		unsigned int get_board_position();
		void update(float deltaTime) override;

		Animator* mator;
		Animation* mation;
		/*void draw(Renderer& renderer) override;
		void play_animation();*/
	private:
		int boardPosition;
		int moves = 0;
		float startTime;
		
};



#endif // !PLAYER_H
