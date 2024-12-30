#include"Camera.h"

Camera::Camera()
{
	cameraPos = glm::vec3(0.0f, 50.0f, 150.0f);
	setCameraToLookAtOrigin();
}

glm::vec3 Camera::getCameraPos()
{
	return cameraPos;
}

void Camera::setCameraPos(glm::vec3 pos)
{
	cameraPos = pos;
}

glm::vec3 Camera::getCameraFront()
{
	return cameraFront;
}

void Camera::setCameraFront(glm::vec3 front)
{
	cameraFront = front;
}

glm::vec3 Camera::getCameraUp()
{
	return cameraUp;
}

void Camera::setCameraUp(glm::vec3 up)
{
	cameraUp = up;
}

void Camera::setCameraToLookAtOrigin() {
	cameraFront = glm::normalize(glm::vec3(0.0f) - cameraPos); // Pointing to the origin
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Set a standard up vector
}

glm::mat4 Camera::get_view_matrix()
{
	return glm::lookAt(getCameraPos(), getCameraPos() + getCameraFront(), getCameraUp());
	
}

glm::mat4 Camera::get_projection_matrix()
{
	//hard coded aspect ratio, figure out to transform this
	return glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 10000.0f);
}