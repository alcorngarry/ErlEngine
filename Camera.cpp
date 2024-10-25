#include"Camera.h"

Camera::Camera()
{
	cameraPos = glm::vec3(0.0f, 50.0f, 150.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraUp = glm::vec3(0.0f, 25.0f, 0.0f);
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