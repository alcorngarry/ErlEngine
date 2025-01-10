#include"SelectCardLeftCommand.h"

SelectCardLeftCommand::SelectCardLeftCommand(Player* player, float deltaTime) : player(player), deltaTime(deltaTime) {}

void SelectCardLeftCommand::execute() {
	if (player->selectedCardIndex > 0) {
		player->selectedCardIndex--;
	    std::cout << "Current card selected " << player->selectedCardIndex << std::endl;
	}
}