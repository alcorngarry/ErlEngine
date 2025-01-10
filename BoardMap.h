#ifndef BOARD_MAP_H
#define BOARD_MAP_H
#include"Map.h"

class BoardMap : public Map {
	public:
		enum BoardState {
			PLAYER_TURN,
			PLAYER_MOVE
		};

		BoardMap(std::string mapName);
		std::vector<GameObject*> get_board_objects();
		void update(float deltaTime) override;
		void draw(float deltaTime) override;
	private:
		unsigned int currentPlayer = 0;
		void load_players() override;
		void process_input(InputManager* inputManager, float deltaTime) override;
		void process_board_space(unsigned int boardId);
};

#endif // !BOARD_MAP_H
