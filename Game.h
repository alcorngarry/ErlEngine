#pragma once
#ifndef GAME_H
#define GAME_H
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"Shader.h"
#include"Model.h"
#include"Map.h"
#include"Player.h"
#include"Renderer.h"
#include"Camera.h"
#include"AssetManager.h"

enum GameState {
	GAME_ACTIVE
};

class Game {
	public:
		bool Keys[1024];
		bool KeysProcessed[1024];
		bool MouseButtons[7];
		
		std::vector<Map> Maps;
		GameState State;

		Game();
		~Game();

		void init(GLFWwindow* window);
		void process_input(float deltaTime, float yaw, float pitch, float xpos, float ypos);
		void update(float deltaTime);
		void render();
		float roll_dice();
	private:
		void colorPicking(float xpos, float ypos);
};

#endif // !GAME_H