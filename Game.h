#ifndef GAME_H
#define GAME_H
#include<glm/glm.hpp>
#include"Map.h"
#include"BoardMap.h"
#include"PongMap.h"
#include"AssetManager.h"
#include"SaveCommand.h"
#include"MoveCameraBackwardCommand.h"
#include"MoveCameraForwardCommand.h"
#include"MoveCameraLeftCommand.h"
#include"MoveCameraRightCommand.h"
#include"SelectEntityCommand.h"
#include"AddRemoveEntityCommand.h"
#include"MoveEntityCommand.h"

enum GameState {
	GAME_ACTIVE,
	DEBUG_MENU
};

class Game {
	public:
		std::vector<Map*> Maps;
		GameState State;
		short level;

		Game(GLFWwindow* window);
		~Game();

		void init();
		void update(float deltaTime);
		void render(float deltaTime);
		void set_debug_controls(float deltaTime);
	private:
		
};	

#endif // !GAME_H