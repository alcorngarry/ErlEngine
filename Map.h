#ifndef MAP_H
#define MAP_H
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<glm/glm.hpp>
#include"Renderer.h"
#include"AssetManager.h"
#include"InputManager.h"

class Map 
{
	public:
		PlayerControls playerControls{ GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_SPACE, GLFW_KEY_LEFT, GLFW_KEY_RIGHT };

		enum State {
			DEFAULT,
			DEBUG
		};

		enum LoadState {
			KEEP_MAP,
			CHANGE_MAP
		};

		LoadState loadState;
		State state;
		std::vector<GameObject*> entities;
		std::vector<GameObject*> lights;
		std::vector<GameObject*> boardSpaces;
		std::vector<Player*> players;
		SkyBox* skybox;
		Camera* camera;

		Map(std::string mapName);
		void save();
		void load();
		virtual void draw(float deltaTime);
		virtual void set_controls(float deltaTime) = 0; //interface method signature
		virtual void update(float deltaTime) = 0; //interface method signature
		//move to editor class
		void duplicate_model(int selectedIndex);
		void remove_model(int selectedIndex);
	protected:
		std::ofstream writeMap;
		std::ifstream readMap;
		std::string fileName;
		virtual void load_players();
	private:
		void load_skybox();
};

#endif // !MAP_H
