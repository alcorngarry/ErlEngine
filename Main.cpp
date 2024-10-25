#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>

#include"Game.h"

#include<GLFW/glfw3.h>

void get_resolution();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

unsigned int window_width, window_height;
float xpos, ypos;
bool firstMouse = true;
float lastX, lastY;
float yaw = -90.0f, pitch = 0.0f;

glm::vec3 front;
glm::mat4 projection;

Game partyGame;

int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//void get_resolution();
	window_width = 1024;
	window_height = 768;

	lastX = window_width / 2.0;
	lastY = window_height / 2.0;
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "test_game", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	partyGame.init(window);

	float lastFrame = 0.0f;
	float deltaTime = 0.0f;

	// game loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		partyGame.process_input(deltaTime, yaw, pitch, xpos, ypos);

		partyGame.update(deltaTime);

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		partyGame.render();

		glfwSwapBuffers(window);
	}

	partyGame.Maps[0].save();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void get_resolution()
{
	//const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	/*window_width = mode->width;
	window_height = mode->height;*/

	window_width = 1024;
	window_height = 768;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	xpos = static_cast<float>(xposIn);
	ypos = static_cast<float>(yposIn);

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

	//front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//front.y = sin(glm::radians(pitch));
	//front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//front = glm::normalize(front);

	//glm::vec4 mouse_dir = glm::vec4((2.0f * xpos) / window_width - 1.0f, 1.0f - (2.0f * ypos) / window_height, -1.0f, 1.0f);
	//glm::mat4 inverseProjMatrix = glm::inverse(projection);
	//glm::vec4 ray_eye = mouse_dir * inverseProjMatrix;
	//ray_eye.z = -1.0f;
	//ray_eye.w = 0.0f;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		partyGame.MouseButtons[button] = !partyGame.MouseButtons[button];
	}
	else if (action == GLFW_RELEASE)
	{
		partyGame.MouseButtons[button] = !partyGame.MouseButtons[button];
	}
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			partyGame.Keys[key] = true;
		else if (action == GLFW_RELEASE)
		{
			partyGame.Keys[key] = false;
			partyGame.KeysProcessed[key] = false;
		}
	}
}