#pragma once
#include"InputManager.h"

bool InputManager::Keys[1024] = { false };
bool InputManager::KeysProcessed[1024] = { false };
bool InputManager::MouseButtons[7] = { false };

InputManager::InputManager()
    : window(nullptr),
    xpos(0.0f),
    ypos(0.0f),
    firstMouse(true),
    lastX(0.0f),
    lastY(0.0f),
    yaw(-90.0f),
    pitch(0.0f) {
}

InputManager::InputManager(GLFWwindow* window) {
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    this->xpos = 0.0f;
    this->ypos = 0.0f;
    this->window = window;
    this->lastX = windowWidth / 2.0f;
    this->lastY = windowHeight / 2.0f;
    this->firstMouse = true; 
}

void InputManager::update() {
    glfwGetCursorPos(window, &xpos, &ypos);

    if (firstMouse) {
        this->lastX = xpos;
        this->lastY = ypos;
        this->firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Inverted Y-axis
    this->lastX = xpos;
    this->lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    if (pitch > 89.0f) this->pitch = 89.0f;
    if (pitch < -89.0f) this->pitch = -89.0f;

    // Poll mouse button states
    for (int button = 0; button < 7; ++button) {
        this->MouseButtons[button] = glfwGetMouseButton(window, button) == GLFW_PRESS;
    }

    // Poll key states
    for (int key = 0; key < 1024; ++key) {
        int state = glfwGetKey(window, key);
        if (state == GLFW_PRESS) {
            if (!this->Keys[key]) { // Key was not previously pressed
                this->Keys[key] = true;
                this->KeysProcessed[key] = false; // Mark as not processed
            }
        }
        else if (state == GLFW_RELEASE) {
            if (this->Keys[key]) {
                this->KeysProcessed[key] = true; // Mark as processed
                this->Keys[key] = false;
            }
            
        }
    }

    // Close window if ESC is pressed
    if (this->Keys[GLFW_KEY_ESCAPE]) {
        glfwSetWindowShouldClose(window, true);
    }
}