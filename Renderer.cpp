#include"Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Renderer::Renderer(Shader &shader, )
//{
//	this->shader = shader;
//}
//
//Renderer::~Renderer()
//{
//}
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Shader shaderProgram;
Shader lightShaderProgram;
Shader skinnedShaderProgram;
Shader pickingShaderProgram;

GLFWwindow* window;
DebugMenu debugMenu;

std::vector<Player> m_players;
std::vector<GameObject> m_entities;
std::vector<GameObject> m_lights;
Camera m_camera;

int selectedIndex = -1;

int windowWidth = 1920;
int windowHeight = 1080;

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	debugMenu = DebugMenu(window);

	shaderProgram = Shader("default.vert.glsl", "default.frag.glsl");
	lightShaderProgram = Shader("light.vert.glsl", "light.frag.glsl");
	skinnedShaderProgram = Shader("skinned.vert.glsl", "skinned.frag.glsl");
	pickingShaderProgram = Shader("picking.vert.glsl", "picking.frag.glsl");
	
	return window;
}

void Renderer::create_renderer(std::vector<Player> players, std::vector<GameObject> entities, std::vector<GameObject> lights, Camera camera)
{
	m_players = players;
	m_entities = entities;
	m_lights = lights;
	m_camera = camera;

	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 10000.0f);

	skinnedShaderProgram.use();
	glUniformMatrix4fv(glGetUniformLocation(skinnedShaderProgram.ID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skinnedShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	for (Player player : players)
	{
		Renderer::draw_player(player.GameModel, player.Position, player.Size, player.Rotation, player.transform);
	}

	shaderProgram.use();
	glUniformMatrix4fv(glGetUniformLocation(skinnedShaderProgram.ID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skinnedShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	shaderProgram.setVec3("lightPos", lights.at(0).Position);
	shaderProgram.setVec3("lightColor", glm::vec3(1.0f));
	for (int i = 0; i < entities.size(); i++)
	{
		shaderProgram.setBool("selected", i == Renderer::get_selected_index());
		Renderer::draw_entity(entities.at(i).GameModel, entities.at(i).Position, entities.at(i).Size, entities.at(i).Rotation);
	}

	lightShaderProgram.use();
	glUniformMatrix4fv(glGetUniformLocation(skinnedShaderProgram.ID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skinnedShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	lightShaderProgram.setVec3("lightColor", glm::vec3(1.0f));
	for (GameObject light : lights)
	{
		Renderer::draw_light(light.GameModel, light.Position, light.Size, light.Rotation);
	}
}

void Renderer::draw_player(Model& model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, std::vector<glm::mat4> transform)
{
	
	glm::mat4 modelMat = glm::mat4(1.0f);

	for (int i = 0; i < transform.size(); ++i)
	{
		skinnedShaderProgram.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transform[i]);
	}

	modelMat = glm::translate(modelMat, pos);

	modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat = glm::scale(modelMat, size);

	skinnedShaderProgram.setMat4("model", modelMat);

	model.draw(skinnedShaderProgram);
}

void Renderer::draw_entity(Model& model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation)
{
	glm::mat4 modelMat = glm::mat4(1.0f);
	
	modelMat = glm::translate(modelMat, pos);
	
	modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat = glm::scale(modelMat, size);

	shaderProgram.setMat4("model", modelMat);
	model.draw(shaderProgram);
}

void Renderer::draw_light(Model& model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation)
{
	glm::mat4 modelMat = glm::mat4(1.0f);

	modelMat = glm::translate(modelMat, pos);

	modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat = glm::scale(modelMat, size);

	shaderProgram.setMat4("model", modelMat);
	model.draw(shaderProgram);
}

void Renderer::create_menu(float deltaTime)
{
	debugMenu.create_menu(m_entities, m_camera, deltaTime);
}

//void Renderer::draw(Model &model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation)
//{
//	glm::mat4 modelMat = glm::mat4(1.0f);
//
//	modelMat = glm::translate(modelMat, pos);
//
//	modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
//	modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
//	modelMat = glm::scale(modelMat, size);
//
//	//this->shader.setMat4("finalBonesMatrices[" + std::to_string(0) + "]", glm::mat4(0));
//
//	this->shader.setMat4("model", modelMat);
//	model.draw(shader);
//}
//
//void Renderer::draw(Model& model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, std::vector<glm::mat4> transform)
//{
//	glm::mat4 modelMat = glm::mat4(1.0f);
//
//	for (int i = 0; i < transform.size(); ++i)
//	{
//		this->shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transform[i]);
//	}
//
//	modelMat = glm::translate(modelMat, pos);
//
//	modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
//	modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
//	modelMat = glm::scale(modelMat, size);
//
//	this->shader.setMat4("model", modelMat);
//
//	model.draw(shader);
//}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Renderer::select_entity(float xpos, float ypos)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//-------duplicate fix eventually
	glm::mat4 view = glm::lookAt(m_camera.getCameraPos(), m_camera.getCameraPos() + m_camera.getCameraFront(), m_camera.getCameraUp());
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 10000.0f);
	//
	pickingShaderProgram.use();
	glEnableVertexAttribArray(0);
	glUniformMatrix4fv(glGetUniformLocation(pickingShaderProgram.ID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(pickingShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	for (int i = 0; i < m_entities.size(); i++)
	{
		int r = (i & 0x000000FF) >> 0;
		int g = (i & 0x0000FF00) >> 8;
		int b = (i & 0x00FF0000) >> 16;

		pickingShaderProgram.setVec4("PickingColor", glm::vec4((float)r / 255.0, (float)g / 255.0, (float)b / 255.0, 1.0f));
		Renderer::draw_entity(m_entities.at(i).GameModel, m_entities.at(i).Position, m_entities.at(i).Size, m_entities.at(i).Rotation);
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