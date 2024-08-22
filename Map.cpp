#include"Map.h"
#include <string>

Map::Map(std::string mapName)
{
	fileName = mapName;
}

void Map::save(glm::vec3 objLoc)
{
	writeMap = std::ofstream{ fileName + ".txt" };
	writeMap << objLoc.x << "," << objLoc.y << "," << objLoc.z << std::endl;
	writeMap.close();
}

glm::vec3 Map::load()
{
	readMap.open(fileName + ".txt");
	std::string line;
	float x, y, z;

	getline(readMap, line, ',');

	if (line.length() > 0)
	{
		x = std::stof(line);

		// Parse y
		getline(readMap, line, ',');
		y = std::stof(line);

		// Parse z
		getline(readMap, line, ',');
		z = std::stof(line);

		readMap.close();
		// Return the constructed glm::vec3
		return glm::vec3(x, y, z);
	}
	
	return glm::vec3(0.0f, 0.0f, 0.0f);
}