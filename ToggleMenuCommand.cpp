#include"ToggleMenuCommand.h"

ToggleMenuCommand::ToggleMenuCommand(Game* game) : game(game) {}

void ToggleMenuCommand::execute() {
    game->State = (game->State == DEBUG_MENU) ? GAME_ACTIVE : DEBUG_MENU;
}