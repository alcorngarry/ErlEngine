#include"AddRemoveEntityCommand.h"

AddRemoveEntityCommand::AddRemoveEntityCommand(Map* map, bool isAdd) : map(map), isAdd(isAdd) {}

void AddRemoveEntityCommand::execute()
{
	if (isAdd)
	{
		map->duplicate_model(Renderer::get_selected_index());
	}
	else {
		map->remove_model(Renderer::get_selected_index());
	}
	
}