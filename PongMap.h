#pragma once
#ifndef PONG_MAP_H
#include"Map.h"

class PongMap : public Map {
	public:
		PongMap(std::string mapName, DebugMenu debugMenu);
		void process_input(InputManager& inputManager, float deltaTime) override;
		void update(float deltaTime) override;
	protected:
		void load_players(AssetManager assetManager) override;
	private:
		void player_collision(Player* player);
};
#endif // !PONG_MAP_H
