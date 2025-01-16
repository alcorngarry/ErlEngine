#include"MoveCameraLeftCommand.h"

MoveCameraLeftCommand::MoveCameraLeftCommand(Camera* camera, float deltaTime) : camera(camera), deltaTime(deltaTime) {}

void MoveCameraLeftCommand::execute() {
	float cameraSpeed = static_cast<float>(0.5f * deltaTime);
	camera->setCameraPos(camera->getCameraPos() - glm::normalize(glm::cross(camera->getCameraFront(), camera->getCameraUp())) * cameraSpeed);

}