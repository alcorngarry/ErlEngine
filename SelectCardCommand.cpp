#include"SelectCardCommand.h"

SelectCardCommand::SelectCardCommand(Player* player) : player(player){}

void SelectCardCommand::execute() {
    if (!player->inMotion) {
        player->inMotion = true;
        player->start_move(static_cast<float>(glfwGetTime()), player->get_cards()[player->get_selected_card_index()]);
    }
}