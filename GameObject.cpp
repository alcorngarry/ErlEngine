#include"GameObject.h"

GameObject::GameObject(unsigned int id, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation) : GameModel(model), Position(pos), Size(size), Rotation(rotation)
{
	this->id = id;
}

GameObject::GameObject()
{
}

void GameObject::update(float deltaTime)
{

}

glm::vec3 GameObject::get_aabb_max() const
{
	return local_to_world(GameModel->getMaxAABB());
}

glm::vec3 GameObject::get_aabb_min() const
{
	return local_to_world(GameModel->getMinAABB());
}

glm::vec3 GameObject::local_to_world(const glm::vec3& localPos) const
{
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), Position);
	modelMatrix = glm::scale(modelMatrix, Size);
	glm::vec4 worldPos = modelMatrix * glm::vec4(localPos, 1.0f);

	return glm::vec3(worldPos);
}