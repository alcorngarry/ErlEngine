#pragma once
#ifndef DEBUGMENU_H
#include <glm/gtc/type_ptr.hpp>
#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
#include<string>
#include<vector>

#include"Renderer.h"

class DebugMenu
{
	public:
		DebugMenu(GLFWwindow* glfwWindow);
		DebugMenu();

		~DebugMenu();
		void create_menu(std::vector<GameObject*> entities, Camera* camera, float deltaTime, int roll = -1);
		void shut_down();
};


#endif // !DEBUGMENU_H
