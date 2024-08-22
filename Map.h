#pragma once
#ifndef MAP_H
#include<iostream>
#include<fstream>
#include<glm/glm.hpp>

class Map 
{
	public:
		Map(std::string mapName);
		void save(glm::vec3 objLoc);
		glm::vec3 load();
	private:
		std::ofstream writeMap;
		std::ifstream readMap;
		std::string fileName;
};

#endif // !MAP_H
