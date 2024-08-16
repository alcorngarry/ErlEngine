#include "CharacterController.h"


CharacterController::CharacterController()
{
	isWireframe = false;
	cameraPos = glm::vec3(0.0f, 10.0f, 30.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraUp = glm::vec3(0.0f, 5.0f, 0.0f);
	isMenuOpen = false;
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

void CharacterController::proccessInput(GLFWwindow* window, float deltaTime, glm::vec3& objectPos, float& rotationY, float& rotationX)
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

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		isMenuOpen = !isMenuOpen;
		if (isMenuOpen)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	if (glfwJoystickPresent(GLFW_JOYSTICK_1) && isMenuOpen == false)
	{
		int axesCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

		if (rotationY > 6.28319f || rotationY < -6.28319f)
		{
			//save from overflow lol
			rotationY = 0.0f;
		}
		rotationY -= axes[2] * 0.01f;
		rotationX -= axes[3] * 0.005f;

		float camX = sin(rotationY) * 20;
		float camZ = cos(rotationY) * 20;

		objectPos.x += sin(rotationY) * axes[1] * 0.1f;
		objectPos.z += cos(rotationY) * axes[1] * 0.1f;

		cameraPos.x = objectPos.x + camX;
		cameraPos.z = objectPos.z + camZ;
		cameraPos.y = cos(rotationX) * 20;
	}
}