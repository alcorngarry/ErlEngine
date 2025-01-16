#include"MoveCameraForwardCommand.h"

MoveCameraForwardCommand::MoveCameraForwardCommand(Camera* camera, float deltaTime) : camera(camera), deltaTime(deltaTime){}

void MoveCameraForwardCommand::execute() {
	float cameraSpeed = static_cast<float>(0.5f * deltaTime);
	camera->setCameraPos(camera->getCameraPos() + cameraSpeed * camera->getCameraFront());
}