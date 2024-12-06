#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include"Model.h"

class GameObject {
	public:
		glm::vec3 Position, Size, Rotation;
		unsigned int id;
		Model GameModel;
		GameObject(unsigned int id, Model model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
		GameObject();

		//virtual void draw(Renderer& renderer);
		virtual void update(float deltaTime);
		//void draw_bounding_box(Renderer& renderer);
};

#endif // !GAME_OBJECT_H
