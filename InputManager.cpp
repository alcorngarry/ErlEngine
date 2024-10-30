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
    // Optionally initialize other members as needed
}

InputManager::InputManager(GLFWwindow* window) {
    glfwSetWindowUserPointer(window, this); // Set user pointer to access the instance

    // Store the window for polling
    this->window = window;

    lastX = 1024 / 2.0f;
    lastY = 768 / 2.0f;
    firstMouse = true; // Initialize firstMouse
}

void InputManager::Update() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    xpos = static_cast<float>(xpos);
    ypos = static_cast<float>(ypos);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Inverted Y-axis
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Poll mouse button states
    for (int button = 0; button < 7; ++button) {
        MouseButtons[button] = glfwGetMouseButton(window, button) == GLFW_PRESS;
    }

    // Poll key states
    for (int key = 0; key < 1024; ++key) {
        int state = glfwGetKey(window, key);
        if (state == GLFW_PRESS) {
            Keys[key] = true;
            KeysProcessed[key] = false; // Reset processed state
        }
        else if (state == GLFW_RELEASE) {
            if (Keys[key]) {
                KeysProcessed[key] = true; // Mark as processed
            }
            Keys[key] = false;
        }
    }

    // Close window if ESC is pressed
    if (Keys[GLFW_KEY_ESCAPE]) {
        glfwSetWindowShouldClose(window, true);
    }
}