#pragma once
#ifndef GAME_H
#define GAME_H
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"Shader.h"
#include"Model.h"
#include"Map.h"
#include"BoardMap.h"
#include"Player.h"
#include"Renderer.h"
#include"Camera.h"
#include"AssetManager.h"
#include"DebugMenu.h"
#include"PongMap.h"
#include"InputManager.h"

enum GameState {
	GAME_ACTIVE,
	DEBUG_MENU
};

class Game {
	public:
		unsigned int level;
		
		std::vector<Map*> Maps;
		GameState State;

		Game(GLFWwindow* window);
		~Game();

		void init(GLFWwindow* window);
		void process_input(float deltaTime);
		void update(float deltaTime);
		void render(float deltaTime);
	private:
		InputManager inputManager;
		int windowWidth = 1024;
		int windowHeight = 768;
};

#endif // !GAME_H