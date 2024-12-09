#pragma once
#ifndef MAP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include<glm/glm.hpp>
#include"GameObject.h"
#include"Renderer.h"
#include"AssetManager.h"
#include"Camera.h"
#include"InputManager.h"
#include"DebugMenu.h"
#include"Player.h"

class Map 
{
	public:
		Map(std::string mapName, DebugMenu debugMenu);
		Map();
		std::vector<GameObject> entities;
		std::vector<GameObject> lights;
		Camera camera;

		void save();
		void load(AssetManager assetManager);
		void draw(Renderer& renderer, bool isLight, float deltaTime);
		virtual void process_input(InputManager& inputManager, float deltaTime);
		virtual void update();
	protected:
		std::ofstream writeMap;
		std::ifstream readMap;
		std::string fileName;
		int ballVal;
		DebugMenu debugMenu;
		Player* player1 = nullptr;
		Player* player2 = nullptr;
		Player* player3 = nullptr;
		Player* player4 = nullptr;

		void duplicate_model(int selectedIndex);
		void remove_model(int selectedIndex);
		virtual void load_players(AssetManager assetManager);

		void menu_input(InputManager& inputManager, float deltaTime);

};

#endif // !MAP_H
