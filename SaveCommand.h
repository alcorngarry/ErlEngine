#ifndef SAVE_COMMAND_H
#define SAVE_COMMAND_H
#include"Command.h"
#include"Map.h"

class SaveCommand : public Command {
public:
    SaveCommand(Map* map);
    void execute() override;

private:
    Map* map;
};
#endif // !SAVE_COMMAND_H