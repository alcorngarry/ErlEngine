#include"Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(Shader &shader)
{
	this->shader = shader;
}

Renderer::~Renderer()
{
}

void Renderer::draw(Model &model, glm::vec3 pos, glm::vec3 size)
{
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::scale(modelMat, size);
	modelMat = glm::translate(modelMat, pos);
	this->shader.setMat4("model", modelMat);
	model.draw(shader);
}

void Renderer::initRenderer()
{

}
