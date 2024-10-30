#pragma once
#ifndef INPUT_MANAGER_H
#include<GLFW/glfw3.h>

class InputManager {
public:
    InputManager(GLFWwindow* window);
    InputManager();

    static bool Keys[1024];
    static bool KeysProcessed[1024];
    static bool MouseButtons[7];

    float xpos, ypos;
    bool firstMouse = true;
    float lastX, lastY;
    float yaw = -90.0f, pitch = 0.0f;

    // New method for updating input state
    void Update();

private:
    GLFWwindow* window;
};

#endif // !INPUT_MANAGER_H
