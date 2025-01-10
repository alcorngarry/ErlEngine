#ifndef SELECT_CARD_R_COMMAND_H
#define SELECT_CARD_R_COMMAND_H
#include "Command.h"
#include"Player.h"

class SelectCardRightCommand : public Command {
public:
    SelectCardRightCommand(Player* player, float deltaTime);
    void execute() override;

private:
    Player* player;
    float deltaTime;
};
#endif // !SELECT_CARD_R_COMMAND_H