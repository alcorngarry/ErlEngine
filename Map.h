#pragma once
#ifndef MAP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include<glm/glm.hpp>
#include"Renderer.h"
#include"AssetManager.h"
#include"InputManager.h"

class Map 
{
	public:
		enum State {
			MENU_OPEN,
			MENU_CLOSED
		};

		State State;
		Map(std::string mapName);
		Map();
		
		std::vector<GameObject> entities;
		std::vector<GameObject> lights;
		std::vector<Player> players;
		
		Camera camera;

		void save();
		void load(AssetManager assetManager);
		//void draw(Renderer& renderer, bool isLight, float deltaTime);
		void draw(float deltaTime);
		virtual void process_input(InputManager& inputManager, float deltaTime);
		virtual void update(float deltaTime) = 0; //interface method signature
	protected:
		std::ofstream writeMap;
		std::ifstream readMap;
		std::string fileName;

		void duplicate_model(int selectedIndex);
		void remove_model(int selectedIndex);
		virtual void load_players(AssetManager assetManager);
		void menu_input(InputManager& inputManager, float deltaTime);
};

#endif // !MAP_H
