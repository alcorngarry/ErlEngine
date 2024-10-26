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

class Map 
{
	public:
		std::vector<GameObject> entities;

		Map(std::string mapName);

		void save();
		void load(AssetManager assetManager);
		void draw(Renderer& renderer, int selectedIndex, bool isLight);
		void draw_picking(Renderer& renderer);
		void duplicate_model(int selectedIndex);
		void remove_model(int selectedIndex);

		std::vector<glm::vec3> get_board_positions();
	private:
		std::vector<glm::vec3> mapAssets;
		std::ofstream writeMap;
		std::ifstream readMap;
		std::string fileName;
};

#endif // !MAP_H
