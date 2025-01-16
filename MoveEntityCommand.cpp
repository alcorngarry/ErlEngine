#include"MoveEntityCommand.h"

MoveEntityCommand::MoveEntityCommand(GameObject* entity, int axis, float moveSpeed) : entity(entity), axis(axis), moveSpeed(moveSpeed) {}

void MoveEntityCommand::execute() 
{
	if (axis == 0)
	{
		entity->Position.x += moveSpeed;
	}
	else if (axis == 1)
	{
		entity->Position.y += moveSpeed;
	}
	else if (axis == 2)
	{
		entity->Position.z += moveSpeed;
	}
}