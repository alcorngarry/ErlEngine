#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>

#include"Game.h"
#include<GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char* argv[])
{
	GLFWwindow* window = Renderer::init_render();

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Game partyGame = Game(window);

	partyGame.init();

	float lastFrame = 0.0f;
	float deltaTime = 0.0f;

	glEnable(GL_DEPTH_TEST);

	// game loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		partyGame.process_input(deltaTime);

		partyGame.update(deltaTime);

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		partyGame.render(deltaTime);

		glfwSwapBuffers(window);
	}

	//for (int i = 0; i < partyGame.Maps.size(); i++)
	//{
	//	partyGame.Maps[i]->save();
	//}

	//partyGame.Maps[1]->save();
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}