#ifndef SELECT_CARD_COMMAND_H
#define SELECT_CARD_COMMAND_H
#include"Command.h"
#include"Player.h"

class SelectCardCommand : public Command {
public:
    SelectCardCommand(Player* player);
    void execute() override;

private:
    Player* player;
};
#endif // !SELECT_CARD_COMMAND_H