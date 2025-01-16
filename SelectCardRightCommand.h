#ifndef SELECT_CARD_R_COMMAND_H
#define SELECT_CARD_R_COMMAND_H
#include "Command.h"
#include"Player.h"

class SelectCardRightCommand : public Command {
public:
    SelectCardRightCommand(Player* player);
    void execute() override;

private:
    Player* player;
};
#endif // !SELECT_CARD_R_COMMAND_H