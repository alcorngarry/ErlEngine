#pragma once
#ifndef MAP_H
#include<iostream>
#include<fstream>
#include<glm/glm.hpp>

class Map 
{
	public:
		Map();
		void save(glm::vec3 objLoc);
		void load();
	private:
		std::ofstream MapFile;
};

#endif // !MAP_H
