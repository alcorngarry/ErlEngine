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

    double xpos, ypos;
    bool firstMouse = true;
    float lastX, lastY;
    float yaw = -90.0f, pitch = 0.0f;

    void update();
    GLFWwindow* window;
private:
    int windowWidth = 1024;
    int windowHeight = 768;
};

#endif // !INPUT_MANAGER_H
