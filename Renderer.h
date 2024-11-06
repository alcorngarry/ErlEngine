#pragma once
#ifndef RENDERER_H
#include"Shader.h"
#include"Model.h"

class Renderer {

	public:
		Shader shader;
		Renderer(Shader &shader);
		~Renderer();
		void draw(Model &model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);

	private:
		void initRenderer();
};

#endif // !RENDERER_H
