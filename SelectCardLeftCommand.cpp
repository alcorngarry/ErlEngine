#include"SelectCardLeftCommand.h"

SelectCardLeftCommand::SelectCardLeftCommand(Player* player) : player(player) {}

void SelectCardLeftCommand::execute() {
	if (player->selectedCardIndex > 0) {
		player->selectedCardIndex--;
	    std::cout << "Current card selected " << player->selectedCardIndex << std::endl;
	}
}