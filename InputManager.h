#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include<map>
#include"MoveDownCommand.h"
#include"MoveUpCommand.h"
#include"SelectCardCommand.h"
#include"SelectCardLeftCommand.h"
#include"SelectCardRightCommand.h"

struct GamepadState {
    bool buttons[15] = { false };
    float axes[6] = { 0.0f }; //(2 for left stick, 2 for right stick, 2 for triggers)
};

/*GLFW_GAMEPAD_BUTTON_A,
GLFW_GAMEPAD_BUTTON_B,
GLFW_GAMEPAD_BUTTON_X,
GLFW_GAMEPAD_BUTTON_Y,
GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
GLFW_GAMEPAD_BUTTON_BACK,
GLFW_GAMEPAD_BUTTON_START,
GLFW_GAMEPAD_BUTTON_GUIDE,
GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
GLFW_GAMEPAD_BUTTON_DPAD_UP,
GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
GLFW_GAMEPAD_BUTTON_DPAD_DOWN
GLFW_GAMEPAD_BUTTON_DPAD_LEFT.*
GLFW_GAMEPAD_AXIS_LEFT_X,
GLFW_GAMEPAD_AXIS_LEFT_Y,
GLFW_GAMEPAD_AXIS_RIGHT_X,
GLFW_GAMEPAD_AXIS_RIGHT_Y,
GLFW_GAMEPAD_AXIS_LEFT_TRIGGER
GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER*/

class InputManager {
public:
    InputManager(GLFWwindow* window);

    static bool Keys[1024];
    static bool KeysProcessed[1024];
    static bool MouseButtons[7];
    GamepadState gamepadStates[4];

    double xpos, ypos;
    bool firstMouse = true;
    float lastX, lastY;
    float yaw = -90.0f, pitch = 0.0f;

    void update();
    GLFWwindow* m_window;
    void update_cursor();
    void set_key_binding(int key, Command* command);
private:
   
    int windowWidth = 1920;
    int windowHeight = 1080;
};

#endif // !INPUT_MANAGER_H
