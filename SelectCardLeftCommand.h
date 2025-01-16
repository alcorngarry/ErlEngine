#ifndef SELECT_CARD_L_COMMAND_H
#define SELECT_CARD_L_COMMAND_H
#include "Command.h"
#include"Player.h"

class SelectCardLeftCommand : public Command {
public:
    SelectCardLeftCommand(Player* player);
    void execute() override;

private:
    Player* player;
};
#endif // !SELECT_CARD_L_COMMAND_H