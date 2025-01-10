#ifndef MOVE_DOWN_COMMAND_H
#define MOVE_DOWN_COMMAND_H
#include "Command.h"
//#include "Player.h"
class Player;

class MoveDownCommand : public Command {
public:
    MoveDownCommand(Player* player, float deltaTime);
    void execute() override;

private:
    Player* player;
    float deltaTime;
};
#endif // !MOVE_DOWN_COMMAND_H