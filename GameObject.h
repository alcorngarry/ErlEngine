#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include"Model.h"
#include"Renderer.h"

class GameObject {
	public:
		glm::vec3 Position, Size;
		unsigned int id;
		Model GameModel;
		GameObject(unsigned int id, Model model, glm::vec3 pos, glm::vec3 size);
		GameObject();
		virtual void draw(Renderer& renderer);
};

#endif // !GAME_OBJECT_H
