#include"GameObject.h"

GameObject::GameObject(unsigned int id, Model model, glm::vec3 pos, glm::vec3 size) : GameModel(model), Position(pos), Size(size)
{
	this->id = id;
}

GameObject::GameObject()
{
}

void GameObject::draw(Renderer& renderer)
{
	renderer.draw(this->GameModel, this->Position, this->Size);
}