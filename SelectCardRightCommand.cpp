#include"SelectCardRightCommand.h"

SelectCardRightCommand::SelectCardRightCommand(Player* player) : player(player) {}

void SelectCardRightCommand::execute() {
	if (player->selectedCardIndex < 4) {
		player->selectedCardIndex++;
	    std::cout << "Current card selected " << player->selectedCardIndex << std::endl;
	}
}