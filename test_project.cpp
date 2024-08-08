#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"Model.h"
#include"CharacterController.h"

#include<GLFW/glfw3.h>

CharacterController controller;
glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);


float deltaTime = 0.0f;
float lastFrame = 0.0f;
float rotation = 0.0f;

int window_width;
int window_height;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void get_resolution();


unsigned int texture1, texture2;

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 60.0f;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	get_resolution();
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "test_game", NULL, NULL);
	
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader shaderProgram("default.vert.glsl", "default.frag.glsl");

	Shader lightShaderProgram("light.vert.glsl", "light.frag.glsl");

	Model backpack((char*)"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/objects/backpack/backpack.obj");
	Model cube((char*)"C:/Dev/assets/cube.glb");
	Model floor((char*)"C:/Dev/assets/plane/plane.obj");

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	controller = CharacterController();
	
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		controller.proccessInput(window, deltaTime, objectPos, rotation);
		
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();

		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::lookAt(controller.getCameraPos(), controller.getCameraPos() + controller.getCameraFront(), controller.getCameraUp());

		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, objectPos);
		model = glm::rotate(model, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		shaderProgram.setMat4("model", model);
		shaderProgram.setVec3("lightColor", lightColor);
		shaderProgram.setVec3("lightPos", glm::vec3(5.0f, 0.0f, 0.0f));
		shaderProgram.setVec3("viewPos", controller.getCameraPos());


		backpack.draw(shaderProgram);

		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(0.0f, -5.0f, 0.0f));
		model2 = glm::scale(model2 , glm::vec3(25.0f, 25.0f, 25.0f));
		shaderProgram.setMat4("model", model2);

		floor.draw(shaderProgram);

		lightShaderProgram.use();

		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 light = glm::mat4(1.0f);
		light = glm::translate(light, glm::vec3(5.0f, 0.0f, 0.0f));
		light = glm::scale(light, glm::vec3(1.0f, 1.0f, 1.0f));

		lightShaderProgram.setMat4("model", light);
		lightShaderProgram.setVec3("lightColor", lightColor);
		
		cube.draw(lightShaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void get_resolution() 
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	window_width = mode->width;
	window_height = mode->height;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	controller.setCameraFront(glm::normalize(front));
}
