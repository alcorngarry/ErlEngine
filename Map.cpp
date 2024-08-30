#include"Map.h"
#include <string>

Map::Map(std::string mapName)
{
	fileName = mapName;
}

void Map::save(std::vector<glm::vec3> objLoc)
{
	writeMap = std::ofstream{ fileName + ".txt" };
	for (int i = 0; i < objLoc.size(); i++)
	{
		writeMap << objLoc.at(i).x << "," << objLoc.at(i).y << "," << objLoc.at(i).z << std::endl;
	}
	writeMap.close();
}

std::vector<glm::vec3> Map::load()
{
	std::vector<glm::vec3> output;
	readMap.open(fileName + ".txt");
	std::string line;
	float x, y, z;

	while (getline(readMap, line, ','))
	{
		//parse x
		x = std::stof(line);

		// Parse y
		getline(readMap, line, ',');
		y = std::stof(line);

		// Parse z
		getline(readMap, line);
		z = std::stof(line);

		// Return the constructed glm::vec3
		output.push_back(glm::vec3(x, y, z));
	}
	if (output.empty())
	{
		output.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	} 
	readMap.close();

	return output;
}