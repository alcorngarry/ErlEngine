#include"Map.h"

bool screenLock = false;

Map::Map(std::string mapName) : State(MENU_CLOSED)
{
	fileName = mapName;
	camera = Camera();
	//debugMenu = debugMenu;
}

Map::Map()
{
}

void Map::save()
{
	writeMap = std::ofstream{ fileName + ".txt" };
	for (int i = 0; i < entities.size(); i++)
	{
		//don't save ball location
		if (entities[i]->id != 5)
		{
			writeMap << entities[i]->id << ", "
				<< entities[i]->Position.x << "," << entities[i]->Position.y << "," << entities[i]->Position.z << ", "
				<< entities[i]->Size.x << "," << entities[i]->Size.y << "," << entities[i]->Size.z << ", "
				<< entities[i]->Rotation.x << "," << entities[i]->Rotation.y << "," << entities[i]->Rotation.z << std::endl;
		}
	}
	for (GameObject* light : lights)
	{
		writeMap << light->id << ", "
			<< light->Position.x << "," << light->Position.y << "," << light->Position.z << ", "
			<< light->Size.x << "," << light->Size.y << "," << light->Size.z << ", "
			<< light->Rotation.x << "," << light->Rotation.y << "," << light->Rotation.z << std::endl;
	}

	writeMap.close();
}

void Map::load(AssetManager assetManager)
{
	readMap.open(fileName + ".txt");
	std::string line;
	float x, y, z, sizeX, sizeY, sizeZ, rotationX, rotationY, rotationZ;
	int id;

	while (getline(readMap, line, ','))
	{
		// Parse id
		id = std::stof(line);

		//parse position
		getline(readMap, line, ',');
		x = std::stof(line);
		getline(readMap, line, ',');
		y = std::stof(line);
		getline(readMap, line, ',');
		z = std::stof(line);

		//parse size
		getline(readMap, line, ',');
		sizeX = std::stof(line);
		getline(readMap, line, ',');
		sizeY = std::stof(line);
		getline(readMap, line, ',');
		sizeZ = std::stof(line);

		//parse rotation
		getline(readMap, line, ',');
		rotationX = std::stof(line);
		getline(readMap, line, ',');
		rotationY = std::stof(line);
		getline(readMap, line);
		rotationZ = std::stof(line);

		if (id == 1)
		{
			this->lights.push_back(new GameObject(id, assetManager.get_model(id), glm::vec3(x, y, z), glm::vec3(sizeX, sizeY, sizeZ), glm::vec3(rotationX, rotationY, rotationZ)));
		}
		else
		{
			entities.push_back(new GameObject(id, assetManager.get_model(id), glm::vec3(x, y, z), glm::vec3(sizeX, sizeY, sizeZ), glm::vec3(rotationX, rotationY, rotationZ)));
		} 
	} 
	readMap.close();

	load_players(assetManager);
}

void Map::load_players(AssetManager assetManager)
{
	for (int i = 0; i < 4; i++)
	{
		players.push_back(new Player(assetManager.get_model(0), glm::vec3(0.0f) * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f), glm::vec3(0.0f)));
	}
	/*player2 = new Player(*assetManager.get_model(0), glm::vec3(0.0f) * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));
	player3 = new Player(*assetManager.get_model(0), glm::vec3(0.0f) * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));
	player4 = new Player(*assetManager.get_model(0), glm::vec3(0.0f) * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));*/
}


void Map::draw(float deltaTime)
{
	//if (isLight)
	//{
	//	for (int i = 0; i < this->lights.size(); i++)
	//	{
	//		this->lights.at(i).draw(renderer);
	//	}
	//}
	//else {
	//	for (int i = 0; i < entities.size(); i++)
	//	{
	//		renderer.shader.setBool("selected", i == debugMenu.get_selected_index());
	//		entities[i]->draw(renderer);
	//	}

		/*players.at(0).draw(renderer);
		players.at(1).draw(renderer);
		players.at(2).draw(renderer);
		players.at(3).draw(renderer);*/

		//doesn't work at the moment
		//players.at(0).draw_bounding_box(renderer);
		//players.at(1).draw_bounding_box(renderer);
		//players.at(2).draw_bounding_box(renderer);
		//players.at(3).draw_bounding_box(renderer);
	//}

	Renderer::render(players, entities, lights, camera);

	if (State == MENU_OPEN)
	{
		Renderer::create_menu(deltaTime);
	}
}

void Map::duplicate_model(int selectedIndex)
{
	entities.push_back(entities[selectedIndex]);
}

void Map::remove_model(int selectedIndex)
{
	entities.erase(entities.begin() + selectedIndex);
}

void Map::process_input(InputManager& inputManager, float deltaTime)
{
	menu_input(inputManager, deltaTime);
}

void Map::menu_input(InputManager& inputManager, float deltaTime)
{
	if (inputManager.Keys[GLFW_KEY_M] && !inputManager.KeysProcessed[GLFW_KEY_M])
	{
		//create debug menu
		State = State == MENU_OPEN ? MENU_CLOSED : MENU_OPEN;
		inputManager.KeysProcessed[GLFW_KEY_M] = true;
	}

	// move to menu mode eventually
	if (State == MENU_OPEN)
	{
		if (inputManager.Keys[GLFW_KEY_F1] && !inputManager.KeysProcessed[GLFW_KEY_F1])
		{
			screenLock = !screenLock;
			inputManager.KeysProcessed[GLFW_KEY_F1] = true;
		}

		if(!screenLock)
		{
			camera.setCameraFront(glm::normalize(glm::vec3(cos(glm::radians(inputManager.yaw)) * cos(glm::radians(inputManager.pitch)), sin(glm::radians(inputManager.pitch)), sin(glm::radians(inputManager.yaw)) * cos(glm::radians(inputManager.pitch)))));

			float cameraSpeed = static_cast<float>(150 * deltaTime);

			if (inputManager.Keys[GLFW_KEY_W])
			{
				camera.setCameraPos(camera.getCameraPos() + cameraSpeed * camera.getCameraFront());
			}
			if (inputManager.Keys[GLFW_KEY_A])
			{
				camera.setCameraPos(camera.getCameraPos() - glm::normalize(glm::cross(camera.getCameraFront(), camera.getCameraUp())) * cameraSpeed);

			}
			if (inputManager.Keys[GLFW_KEY_S])
			{
				camera.setCameraPos(camera.getCameraPos() - cameraSpeed * camera.getCameraFront());

			}
			if (inputManager.Keys[GLFW_KEY_D])
			{
				camera.setCameraPos(camera.getCameraPos() + glm::normalize(glm::cross(camera.getCameraFront(), camera.getCameraUp())) * cameraSpeed);
			}
		}

		if (inputManager.MouseButtons[GLFW_MOUSE_BUTTON_LEFT])
		{
			//essentially the distance function squares the difference of each vertice(xyz) adds them up and returns the sqrroot.
			//sqrt((objectcoord.x - objectpos.x) * *2 + (objectcoord.y - objectpos.y) * *2 + (objectcoord.z - objectpos.z)**2);
			//std::cout << "object cooords:" << min.x << ", " << min.y << ", " << min.z << ", " << std::endl;

			//color picking lol. Slow but not noticeable especially since it's used for map building.

			Renderer::select_entity(inputManager.xpos, inputManager.ypos);
			//debugMenu.select_object(entities, this->camera, inputManager.xpos, inputManager.ypos);
		}

		if (inputManager.MouseButtons[GLFW_MOUSE_BUTTON_RIGHT])
		{
			Renderer::deselect_index();
			//debugMenu.deselect_index();
		}

		int selectedIndex = Renderer::get_selected_index();
		//if (debugMenu.get_selected_index() != -1)
		if (selectedIndex != -1)
		{
			if (inputManager.Keys[GLFW_KEY_UP])
			{
				entities[selectedIndex]->Position.y += .03f;
			}
			if (inputManager.Keys[GLFW_KEY_DOWN])
			{
				entities[selectedIndex]->Position.y -= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_LEFT])
			{
				entities[selectedIndex]->Position.x += .03f;
			}
			if (inputManager.Keys[GLFW_KEY_RIGHT])
			{
				entities[selectedIndex]->Position.x -= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_J])
			{
				entities[selectedIndex]->Position.z += .03f;
			}
			if (inputManager.Keys[GLFW_KEY_K])
			{
				entities[selectedIndex]->Position.z -= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_N] && !inputManager.KeysProcessed[GLFW_KEY_N])
			{
				this->duplicate_model(selectedIndex);
				inputManager.KeysProcessed[GLFW_KEY_N] = true;
			}
			if (inputManager.Keys[GLFW_KEY_R] && !inputManager.KeysProcessed[GLFW_KEY_R])
			{
				this->remove_model(selectedIndex);
				inputManager.KeysProcessed[GLFW_KEY_R] = true;
			}

			if (inputManager.Keys[GLFW_KEY_R] && !inputManager.KeysProcessed[GLFW_KEY_R])
			{
				this->remove_model(selectedIndex);
				inputManager.KeysProcessed[GLFW_KEY_R] = true;
			}

			if (inputManager.Keys[GLFW_KEY_1])
			{
				entities[selectedIndex]->Size.y *= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_2])
			{
				entities[selectedIndex]->Size.y *= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_3])
			{
				entities[selectedIndex]->Size.x *= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_4])
			{
				entities[selectedIndex]->Size.x *= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_5])
			{
				entities[selectedIndex]->Size.z *= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_6])
			{
				entities[selectedIndex]->Size.z *= .03f;
			}
		}
	}
}