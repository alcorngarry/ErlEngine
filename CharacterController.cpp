#include "CharacterController.h"


CharacterController::CharacterController()
{
	isWireframe = false;
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 CharacterController::getCameraPos()
{
	return cameraPos;
}

void CharacterController::setCameraFront(glm::vec3 front)
{
	cameraFront = front;
}

glm::vec3 CharacterController::getCameraUp()
{
	return cameraUp;
}

glm::vec3 CharacterController::getCameraFront()
{
	return cameraFront;
}

void CharacterController::proccessInput(GLFWwindow* window, float deltaTime, glm::vec3& objectPos, float& rotation)
{
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		if (isWireframe)
		{
			isWireframe = false;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			isWireframe = true;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	float cameraSpeed = static_cast<float>(2.5 * deltaTime);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		int axesCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

		objectPos.x += axes[0] * 0.01f;
		objectPos.y -= axes[1] * 0.01f;

		rotation += axes[2] * 0.01f;
	}
}