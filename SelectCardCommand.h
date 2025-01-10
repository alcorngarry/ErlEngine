#ifndef SELECT_CARD_COMMAND_H
#define SELECT_CARD_COMMAND_H
#include"Command.h"
#include"Player.h"

class SelectCardCommand : public Command {
public:
    SelectCardCommand(Player* player, float deltaTime);
    void execute() override;

private:
    Player* player;
    float deltaTime;
};
#endif // !SELECT_CARD_COMMAND_H