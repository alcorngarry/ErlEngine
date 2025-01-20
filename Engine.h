#pragma once
#ifndef ENGINE_H
#define ENGINE_H
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<functional>
#include"DebugMenu.h"
#include"InputManager.h"
#include"Game.h"
#include"ToggleMenuCommand.h"
#include"CloseWindowCommand.h"

namespace Engine {
	void run();
	void start_game_loop();
};

#endif // !ENGINE_H