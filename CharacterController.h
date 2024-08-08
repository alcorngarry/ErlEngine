#pragma once
#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

class CharacterController
{
	public:
		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;

		CharacterController();
		glm::vec3 getCameraPos();
		void setCameraFront(glm::vec3 front);
		glm::vec3 getCameraUp();
		glm::vec3 getCameraFront();
		void proccessInput(GLFWwindow* window, float deltaTime, glm::vec3& objectPos, float &rotation);

	private:
		bool isWireframe;
};




#endif // !CHARACTERCONTROLLER_H
