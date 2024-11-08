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

void GameObject::draw_bounding_box(Renderer& renderer)
{
	GameModel.getMaxAABB() * 50.0f;
	GameModel.getMinAABB() * 50.0f;

	// Define the vertices and indices for the faceless cube (edges only)
	const GLfloat vertices[] = {
		GameModel.getMinAABB().x, GameModel.getMinAABB().y, GameModel.getMinAABB().z, // 0
		GameModel.getMinAABB().x, GameModel.getMinAABB().y, GameModel.getMaxAABB().z, // 1
		GameModel.getMinAABB().x, GameModel.getMaxAABB().y, GameModel.getMinAABB().z, // 2
		GameModel.getMinAABB().x, GameModel.getMaxAABB().y, GameModel.getMaxAABB().z, // 3
		GameModel.getMaxAABB().x, GameModel.getMinAABB().y, GameModel.getMinAABB().z, // 4
		GameModel.getMaxAABB().x, GameModel.getMinAABB().y, GameModel.getMaxAABB().z, // 5
		GameModel.getMaxAABB().x, GameModel.getMaxAABB().y, GameModel.getMinAABB().z, // 6
		GameModel.getMaxAABB().x, GameModel.getMaxAABB().y,GameModel.getMaxAABB().z  // 7
	};

	// Define the indices for drawing the edges
	const GLuint indices[] = {
		// Front face
		0, 1, 1, 3, 3, 2, 2, 0,

		// Back face
		4, 5, 5, 7, 7, 6, 6, 4,

		// Sides
		0, 4, 1, 5, 2, 6, 3, 7
	};

	// Create and bind VAO, VBO, and EBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VAO

	// Use the shader program
	//renderer.shader.setMat4("model", GameModel)
	renderer.shader.use();

	// Bind VAO and draw the cube
	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0); // Draw 24 lines (edges of the cube)
	glBindVertexArray(0);

	// Cleanup
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
}
