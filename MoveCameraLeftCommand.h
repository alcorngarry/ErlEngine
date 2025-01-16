#ifndef MOVE_CAMERA_LEFT_COMMAND_H
#define MOVE_CAMERA_LEFT_COMMAND_H
#include "Command.h"
#include "Camera.h"

class MoveCameraLeftCommand : public Command {
public:
    MoveCameraLeftCommand(Camera* camera, float deltaTime);
    void execute() override;

private:
    Camera* camera;
    float deltaTime;
};
#endif // !MOVE_CAMERA_LEFT_COMMAND_H