#include"GameObject.h"

GameObject::GameObject(unsigned int id, Model model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation) : GameModel(model), Position(pos), Size(size), Rotation(rotation)
{
	this->id = id;
}

GameObject::GameObject()
{
}

void GameObject::draw(Renderer& renderer)
{
	renderer.draw(this->GameModel, this->Position, this->Size, this->Rotation);
}