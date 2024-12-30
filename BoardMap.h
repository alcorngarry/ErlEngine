#pragma once
#ifndef BOARD_MAP_H
#include"Map.h"

class BoardMap : public Map {
	public:
		BoardMap(std::string mapName);
		float roll_dice();
		std::vector<glm::vec3> get_board_positions();
		void update(float deltaTime) override;
	private:
		void load_players() override;
		void process_input(InputManager& inputManager, float deltaTime) override;
};

#endif // !BOARD_MAP_H
