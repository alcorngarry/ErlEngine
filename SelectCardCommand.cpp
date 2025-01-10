#include"SelectCardCommand.h"

SelectCardCommand::SelectCardCommand(Player* player, float deltaTime) : player(player), deltaTime(deltaTime) {}

void SelectCardCommand::execute() {
    if (!player->inMotion) {
        player->inMotion = true;
        player->start_move(static_cast<float>(glfwGetTime()), player->get_cards()[player->get_selected_card_index()]);
    }
}