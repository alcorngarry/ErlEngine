#pragma once
#ifndef DEBUGMENU_H
#include <glm/gtc/type_ptr.hpp>
#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
#include<string>
#include<vector>

#include"GameObject.h"
#include"Camera.h"
#include"Renderer.h"

class DebugMenu
{
	public:
		DebugMenu(GLFWwindow* glfwWindow);
		DebugMenu();

		~DebugMenu();
	
		void select_object(std::vector<GameObject> entities, Camera camera, float xpos, float ypos);
		void create_menu(std::vector<GameObject> entities, Camera camera, float deltaTime);
		void shut_down();
		void deselect_index();
		int get_selected_index();
	private:
		GLFWwindow* window;
		Renderer* pickingRenderer;
		int selectedIndex;
		void draw_picking(Renderer& renderer, std::vector<GameObject> entities);
};


#endif // !DEBUGMENU_H
