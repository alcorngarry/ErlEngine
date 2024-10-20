#include"GameObject.h"

GameObject::GameObject(Model model, glm::vec3 pos, glm::vec3 size) : GameModel(model), Position(pos), Size(size)
{
}

void GameObject::draw(Renderer& renderer)
{
	renderer.draw(this->GameModel, this->Position, this->Size);
}