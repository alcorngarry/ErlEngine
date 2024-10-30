#include"BoardMap.h"

BoardMap::BoardMap(std::string mapName, DebugMenu debugMenu) : Map(mapName, debugMenu)
{
}

float BoardMap::roll_dice()
{
	srand(time(0));
	int roll = rand() % 6;

	std::cout << "rolled " << roll + 1 << std::endl;
	return roll;
}

std::vector<glm::vec3> BoardMap::get_board_positions()
{
	std::vector<glm::vec3> output;

	for (GameObject entity : this->entities)
	{
		if (entity.id == 4)
		{
			output.push_back(entity.Position);
		}
	}
	return output;
}