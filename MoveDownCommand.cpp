#include"MoveDownCommand.h"
#include "Player.h" 

MoveDownCommand::MoveDownCommand(Player* player, float deltaTime) : player(player), deltaTime(deltaTime) {}

void MoveDownCommand::execute() {
    player->velocity.x = -100.0f;
    player->update(deltaTime);
}