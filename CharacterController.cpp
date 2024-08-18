#include "CharacterController.h"

CharacterController::CharacterController()
{
}

void CharacterController::proccessInput(GLFWwindow* window, Camera& camera, glm::vec3& objectPos, float deltaTime, float& rotationY, float& rotationX)
{
	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
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

		camera.setCameraPos(glm::vec3(objectPos.x + camX, cos(rotationX) * 20, objectPos.z + camZ));
	}
}