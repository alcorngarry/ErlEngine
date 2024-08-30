#pragma once
#ifndef MAP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include<glm/glm.hpp>

class Map 
{
	public:
		Map(std::string mapName);
		void save(std::vector<glm::vec3> objLoc);
		std::vector<glm::vec3> load();
	private:
		std::ofstream writeMap;
		std::ifstream readMap;
		std::string fileName;
};

#endif // !MAP_H
