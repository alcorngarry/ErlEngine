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
		std::vector<glm::mat4> transform;

		Player(Model characterModel, glm::vec3 pos, glm::vec3 size);
		void move_player(std::vector<glm::vec3> spaces);
		void start_move(float startTime, int moves);
		unsigned int get_board_position();
		void update(float deltaTime) override;

		Animator mator;
		Animation mation;
		/*void draw(Renderer& renderer) override;
		void play_animation();*/
	private:
		Model model;
		Shader shader;
		int boardPosition;
		int moves = 0;
		float startTime;
		
};



#endif // !PLAYER_H
