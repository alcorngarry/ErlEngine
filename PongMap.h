#pragma once
#ifndef PONG_MAP_H
#include"Map.h"

class PongMap : public Map {
	public:
		PongMap(std::string mapName);
		void set_controls(float deltaTime) override;
		void update(float deltaTime) override;
	protected:
		void load_players() override;
	private:
		void check_ball_collision(GameObject* entity);

		int team_1_points = 0;
		int team_2_points = 0;
};
#endif // !PONG_MAP_H
