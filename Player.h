#pragma once
#ifndef PLAYER_H
#define GLFW_INCLUDE_NONE
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObject.h"
#include"Model.h"
#include"Shader.h"

class Player : public GameObject {

	public:
		bool inMotion;

		Player(Model characterModel, glm::vec3 pos, glm::vec3 size);
		void move_player(std::vector<glm::vec3> spaces);
		void start_move(float startTime, int moves);
		unsigned int get_board_position();
	private:
		Model model;
		Shader shader;
		int boardPosition;
		int moves = 0;
		float startTime;
};



#endif // !PLAYER_H
