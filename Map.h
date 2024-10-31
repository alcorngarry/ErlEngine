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

class Map 
{
	public:
		Map(std::string mapName, DebugMenu debugMenu);
		Map();

		std::vector<GameObject> entities;
		Camera camera;

		void save();
		void load(AssetManager assetManager);
		void draw(Renderer& renderer, bool isLight, float deltaTime);
		void duplicate_model(int selectedIndex);
		void remove_model(int selectedIndex);
		virtual void process_input(InputManager& inputManager, float deltaTime);
	private:
		std::ofstream writeMap;
		std::ifstream readMap;
		std::string fileName;
		GameObject ball;
		DebugMenu debugMenu;
};

#endif // !MAP_H
