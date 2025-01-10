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
#include"GameObject.h"
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
		InputManager* inputManager;

		Game(GLFWwindow* window);
		~Game();

		void init();
		void process_input(float deltaTime);
		void update(float deltaTime);
		void render(float deltaTime);
};

#endif // !GAME_H