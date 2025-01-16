#ifndef MOVE_CAMERA_FORWARD_COMMAND_H
#define MOVE_CAMERA_FORWARD_COMMAND_H
#include "Command.h"
#include "Camera.h"

class MoveCameraForwardCommand : public Command {
public:
    MoveCameraForwardCommand(Camera* camera, float deltaTime);
    void execute() override;

private:
    Camera* camera;
    float deltaTime;
};
#endif // !MOVE_CAMERA_BACKWARD_COMMAND_H