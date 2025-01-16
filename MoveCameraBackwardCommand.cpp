#include"MoveCameraBackwardCommand.h"

MoveCameraBackwardCommand::MoveCameraBackwardCommand(Camera* camera, float deltaTime) : camera(camera), deltaTime(deltaTime) {}

void MoveCameraBackwardCommand::execute() {
	float cameraSpeed = static_cast<float>(0.5f * deltaTime);
	camera->setCameraPos(camera->getCameraPos() - cameraSpeed * camera->getCameraFront());
}