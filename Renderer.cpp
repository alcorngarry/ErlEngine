#include"Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader* shaderProgram;
Shader* lightShaderProgram;
Shader* skinnedShaderProgram;
Shader* pickingShaderProgram;

GLFWwindow* window;
DebugMenu debugMenu;

std::vector<Player*> m_players;
std::vector<GameObject*> m_entities, m_lights;

Camera m_camera;

glm::mat4 view, projection;

int selectedIndex = -1;
int windowWidth = 1920;
int windowHeight = 1080;

//eventually move this to an engine class....
GLFWwindow* Renderer::init_render()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	void get_resolution();

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	window = glfwCreateWindow(mode->width, mode->height, "test_game", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	debugMenu = DebugMenu(window);

	shaderProgram = new Shader("default.vert.glsl", "default.frag.glsl");
	lightShaderProgram = new Shader("light.vert.glsl", "light.frag.glsl");
	skinnedShaderProgram = new Shader("skinned.vert.glsl", "skinned.frag.glsl");
	pickingShaderProgram = new Shader("picking.vert.glsl", "picking.frag.glsl");
	
	return window;
}

void Renderer::render(std::vector<Player*> players, std::vector<GameObject*> entities, std::vector<GameObject*> lights, Camera& camera)
{
	m_players = players;
	m_entities = entities;
	m_lights = lights;
	m_camera = camera;

	view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
	projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 10000.0f);

	skinnedShaderProgram->use();
	skinnedShaderProgram->setVec3("lightPos", lights[0]->Position);
	skinnedShaderProgram->setVec3("lightColor", glm::vec3(1.0f));
	for (Player* player : players)
	{
		draw_aabb(player->GameModel->getMinAABB(), player->GameModel->getMaxAABB());
		Renderer::draw_skinned(player->GameModel, player->Position, player->Size, player->Rotation, player->transforms);
	}

	shaderProgram->use();
	shaderProgram->setVec3("lightPos", lights[0]->Position);
	shaderProgram->setVec3("lightColor", glm::vec3(1.0f));
	for (int i = 0; i < entities.size(); i++)
	{
		shaderProgram->setBool("selected", i == Renderer::get_selected_index());
		Renderer::draw_static(shaderProgram, entities[i]->GameModel, entities[i]->Position, entities[i]->Size, entities[i]->Rotation);
	}

	lightShaderProgram->use();
	lightShaderProgram->setVec3("lightColor", glm::vec3(1.0f));
	for (GameObject* light : lights)
	{
		Renderer::draw_static(lightShaderProgram, light->GameModel, light->Position, light->Size, light->Rotation);
	}
}

void Renderer::draw_skinned(Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, std::vector<glm::mat4>* transform)
{
	glUniformMatrix4fv(glGetUniformLocation(skinnedShaderProgram->ID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skinnedShaderProgram->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	for (int i = 0; i < transform->size(); ++i)
	{
		skinnedShaderProgram->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transform->at(i));
	}

	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), pos);

	modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat = glm::scale(modelMat, size);

	skinnedShaderProgram->setMat4("model", modelMat);

	model->draw(*skinnedShaderProgram);
}

void Renderer::draw_static(Shader* shader, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation)
{
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), pos);
	
	modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat = glm::scale(modelMat, size);

	shader->setMat4("model", modelMat);
	model->draw(*shaderProgram);
}

void Renderer::create_menu(float deltaTime)
{
	debugMenu.create_menu(m_entities, m_camera, deltaTime);
}

void Renderer::select_entity(float xpos, float ypos)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pickingShaderProgram->use();
	glEnableVertexAttribArray(0);
	glUniformMatrix4fv(glGetUniformLocation(pickingShaderProgram->ID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(pickingShaderProgram->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	for (int i = 0; i < m_entities.size(); i++)
	{
		int r = (i & 0x000000FF) >> 0;
		int g = (i & 0x0000FF00) >> 8;
		int b = (i & 0x00FF0000) >> 16;

		pickingShaderProgram->setVec4("PickingColor", glm::vec4((float)r / 255.0, (float)g / 255.0, (float)b / 255.0, 1.0f));
		Renderer::draw_static(pickingShaderProgram, m_entities[i]->GameModel, m_entities[i]->Position, m_entities[i]->Size, m_entities[i]->Rotation);
		glGetError();
	}

	glDisableVertexAttribArray(0);
	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char data[4];
	std::cout << windowHeight << "\n";
	glReadPixels(xpos, windowHeight - ypos - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	int pickedID = data[0] + (data[1] * 256) + (data[2] * 256 * 256);

	std::cout << xpos << ", " << ypos << std::endl;

	if (!(pickedID == 0x00ffffff))
	{
		std::ostringstream oss;
		oss << "mesh " << pickedID;
		std::cout << oss.str() << std::endl;

		selectedIndex = pickedID;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int Renderer::get_selected_index()
{
	return selectedIndex;
}

void Renderer::deselect_index()
{
	selectedIndex = -1;
}

void Renderer::draw_aabb(const glm::vec3& minAABB,const glm::vec3& maxAABB)
{
	// Define the vertices and indices for the faceless cube (edges only)
	const GLfloat vertices[] = {
		minAABB.x, minAABB.y, minAABB.z, // 0
		minAABB.x, minAABB.y, maxAABB.z, // 1
		minAABB.x, maxAABB.y, minAABB.z, // 2
		minAABB.x, maxAABB.y, maxAABB.z, // 3
		maxAABB.x, minAABB.y, minAABB.z, // 4
		maxAABB.x, minAABB.y, maxAABB.z, // 5
		maxAABB.x, maxAABB.y, minAABB.z, // 6
		maxAABB.x, maxAABB.y,maxAABB.z  // 7
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

	// Bind VAO and draw the cube
	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0); // Draw 24 lines (edges of the cube)
	glBindVertexArray(0);

	// Cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}