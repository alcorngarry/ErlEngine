#include"CloseWindowCommand.h"

CloseWindowCommand::CloseWindowCommand(GLFWwindow* window) : window(window) {}

void CloseWindowCommand::execute()
{
	glfwSetWindowShouldClose(window, true);
}