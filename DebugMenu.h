#pragma once
#ifndef DEBUGMENU_H

#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
#include<string>

#include"CharacterController.h"

class DebugMenu
{
	public:
		DebugMenu();
		DebugMenu(GLFWwindow* window);
		void new_frame();
		void create_menu(float deltaTime, glm::vec3 objectPos, CharacterController controller, float rotationY);
		void shut_down();
};


#endif // !DEBUGMENU_H
