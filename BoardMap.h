#pragma once
#ifndef BOARD_MAP_H
#include"Map.h"

class BoardMap : public Map {
	public:
		BoardMap(std::string mapName, DebugMenu debugMenu);
		float roll_dice();
		std::vector<glm::vec3> get_board_positions();
	private:
};

#endif // !BOARD_MAP_H
