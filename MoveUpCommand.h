#ifndef MOVE_UP_COMMAND_H
#define MOVE_UP_COMMAND_H
#include "Command.h"
#include"Player.h"

class MoveUpCommand : public Command {
public:
    MoveUpCommand(Player* player, float deltaTime);
    void execute() override;

private:
    Player* player;
    float deltaTime;
};
#endif // !MOVE_UP_COMMAND_H