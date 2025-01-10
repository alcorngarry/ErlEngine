#include"InputManager.h"

bool InputManager::Keys[1024] = { false };
bool InputManager::KeysProcessed[1024] = { false };
bool InputManager::MouseButtons[7] = { false };

std::map<int, Command*> keyBindings;

InputManager::InputManager(GLFWwindow* window) {
    xpos = 0.0f;
    ypos = 0.0f;
    m_window = window;
    lastX = windowWidth / 2.0f;
    lastY = windowHeight / 2.0f;
    firstMouse = true;

    //defaults maybe
    /*keyBindings[GLFW_KEY_UP] = new MoveUpCommand();
    keyBindings[GLFW_KEY_DOWN] = new MoveDownCommand();
    keyBindings[GLFW_KEY_LEFT] = new SelectCardLeftCommand();
    keyBindings[GLFW_KEY_RIGHT] = new SelectCardRightCommand();
    keyBindings[GLFW_KEY_SPACE] = new SelectCardCommand();*/
}

void InputManager::update() {

    update_cursor();

    //for (int i = 0; i < sizeof(gamepadStates); i++) {
    //    if (glfwJoystickIsGamepad(i)) {
    //        GLFWgamepadstate state;
    //        if (glfwGetGamepadState(i, &state)) {
    //            for (int button = 0; button < GLFW_GAMEPAD_BUTTON_LAST; ++button) {
    //                gamepadStates[i].buttons[button] = state.buttons[button] == GLFW_PRESS;
    //            }

    //            for (int axis = 0; axis < GLFW_GAMEPAD_AXIS_LAST; ++axis) {
    //                gamepadStates[i].axes[axis] = state.axes[axis];
    //            }
    //        }
    //    }
    //}
    //command structure
    for (auto& binding : keyBindings) {
        int key = binding.first;
        Command* command = binding.second;

        for (int keyIndex = 0; keyIndex < 1024; ++keyIndex) {
            if (glfwGetKey(m_window, key) == GLFW_PRESS && !KeysProcessed[key]) {
                command->execute();
                KeysProcessed[key] = true;
            }
            else if (glfwGetKey(m_window, key) == GLFW_RELEASE) {
                KeysProcessed[key] = false;
            }
        }
    }

    for (int button = 0; button < 7; ++button) {
        MouseButtons[button] = glfwGetMouseButton(m_window, button) == GLFW_PRESS;
    }

    //for (int key = 0; key < 1024; ++key) {
    //    int state = glfwGetKey(m_window, key);
    //    if (state == GLFW_PRESS) {
    //        if (!Keys[key]) {
    //            Keys[key] = true;
    //            KeysProcessed[key] = false;
    //        }
    //    }
    //    else if (state == GLFW_RELEASE) {
    //        if (Keys[key]) {
    //            KeysProcessed[key] = true;
    //            Keys[key] = false;
    //        }

    //    }
    //}

    if (Keys[GLFW_KEY_ESCAPE]) {
        glfwSetWindowShouldClose(m_window, true);
    }
}

void InputManager::set_key_binding(int key, Command* command)
{
    keyBindings[key] = command;
}

void InputManager::update_cursor()
{
    glfwGetWindowSize(m_window, &windowWidth, &windowHeight);
    glfwGetCursorPos(m_window, &xpos, &ypos);

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
}