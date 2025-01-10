#include"MoveUpCommand.h"

MoveUpCommand::MoveUpCommand(Player* player, float deltaTime) : player(player), deltaTime(deltaTime) {}

void MoveUpCommand::execute() {
    player->velocity.x = 100.0f;
    player->update(deltaTime);
}