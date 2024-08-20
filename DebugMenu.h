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
		DebugMenu(GLFWwindow* glfwWindow);
		
		void create_menu(Camera camera, glm::vec3 &objectPos, float deltaTime,  float rotationY);
		void shut_down();
	private:
		bool isWireframe;
		bool isMenuOpen;
		GLFWwindow* window;

		void process_menu_input();
};


#endif // !DEBUGMENU_H
