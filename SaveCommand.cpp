#include"SaveCommand.h"

SaveCommand::SaveCommand(Map* map) : map(map) {}

void SaveCommand::execute() {
	map->save();
	std::cout << "saved current map" << std::endl;
}