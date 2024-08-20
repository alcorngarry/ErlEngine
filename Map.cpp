#include"Map.h"


Map::Map()
{
	MapFile = std::ofstream{ "map_coord_test.txt" };
}

void Map::save(glm::vec3 objLoc)
{
	MapFile << objLoc.x << "," << objLoc.y << "," << objLoc.z << std::endl;
	MapFile.close();
}

void Map::load()
{
	//MapFile.out
}