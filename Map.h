#pragma once
#ifndef MAP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include<glm/glm.hpp>
#include"Renderer.h"
#include"AssetManager.h"
#include"InputManager.h"

class Map 
{
	public:
		enum State {
			MENU_OPEN,
			MENU_CLOSED
		};

		State State;
		Map(std::string mapName);
		Map();
		
		std::vector<GameObject*> entities;
		std::vector<GameObject*> lights;
		std::vector<Player*> players;
		SkyBox* skybox;
		
		Camera camera;

		void save();
		void load();
		void draw(float deltaTime);
		virtual void process_input(InputManager& inputManager, float deltaTime);
		virtual void update(float deltaTime) = 0; //interface method signature
	protected:
		std::ofstream writeMap;
		std::ifstream readMap;
		std::string fileName;

		void duplicate_model(int selectedIndex);
		void remove_model(int selectedIndex);
		virtual void load_players();
		void menu_input(InputManager& inputManager, float deltaTime);
	private:
		void load_skybox();
		//bool AABBRayIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& min, const glm::vec3& max, float& tmin, float& tmax);
		//void check_intersection(float x, float y);

};

#endif // !MAP_H
