#ifndef SELECT_CARD_L_COMMAND_H
#define SELECT_CARD_L_COMMAND_H
#include "Command.h"
#include"Player.h"

class SelectCardLeftCommand : public Command {
public:
    SelectCardLeftCommand(Player* player, float deltaTime);
    void execute() override;

private:
    Player* player;
    float deltaTime;
};
#endif // !SELECT_CARD_L_COMMAND_H