#pragma once
#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include"Camera.h"

class CharacterController
{
	public:
		CharacterController();
		void proccessInput(GLFWwindow* window, Camera& camera, glm::vec3& objectPos, float deltaTime, float &rotationY, float& rotationX);
};
#endif // !CHARACTERCONTROLLER_H
