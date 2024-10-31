#include"Map.h"


Map::Map(std::string mapName, DebugMenu debugMenu)
{
	fileName = mapName;
	camera = Camera();
	debugMenu = debugMenu;
}

Map::Map()
{
}

void Map::save()
{
	writeMap = std::ofstream{ fileName + ".txt" };
	for (int i = 0; i < this->entities.size(); i++)
	{
		writeMap << this->entities.at(i).id << ", " << this->entities.at(i).Position.x << "," << this->entities.at(i).Position.y << "," << this->entities.at(i).Position.z << std::endl;
	}
	writeMap.close();
}

void Map::load(AssetManager assetManager)
{
	readMap.open(fileName + ".txt");
	std::string line;
	float x, y, z;
	int id;

	while (getline(readMap, line, ','))
	{
		// Parse id
		id = std::stof(line);

		//parse x
		getline(readMap, line, ',');
		x = std::stof(line);

		// Parse y
		getline(readMap, line, ',');
		y = std::stof(line);

		// Parse z
		getline(readMap, line);
		z = std::stof(line);

		// Return the constructed glm::vec3
		if (id == 4)
		{
			this->entities.push_back(GameObject(id, *assetManager.get_model(id), glm::vec3(x, y, z), glm::vec3(10.0f)));
		}
		if (id == 1)
		{
			this->entities.push_back(GameObject(id, *assetManager.get_model(id), glm::vec3(x, y, z), glm::vec3(1.0f)));
		}
		if (id == 2)
		{
			this->entities.push_back(GameObject(id, *assetManager.get_model(id), glm::vec3(x, y, z), glm::vec3(1000.0f)));
		}
		if (id == 5)
		{
			ball = GameObject(id, *assetManager.get_model(id), glm::vec3(x, y, z), glm::vec3(5.0f));
			this->entities.push_back(ball);
		}
		if (id == 6)
		{
			this->entities.push_back(GameObject(id, *assetManager.get_model(id), glm::vec3(x, y, z), glm::vec3(1.0f)));
		}
	} 
	readMap.close();
}

void Map::draw(Renderer &renderer, bool isLight, float deltaTime)
{
	if (debugMenu.is_menu_open())
	{
		debugMenu.create_menu(this->entities, this->camera, deltaTime);
	}
	for (int i = 0; i < this->entities.size(); i++)
	{
		if (this->entities.at(i).id == 1)
		{
			if (isLight)
			{
				this->entities.at(i).draw(renderer);
			}
		}
		else {
			renderer.shader.setBool("selected", i == debugMenu.get_selected_index());
			this->entities.at(i).draw(renderer);
		}
	}
}

void Map::duplicate_model(int selectedIndex)
{
	this->entities.push_back(entities[selectedIndex]);
}

void Map::remove_model(int selectedIndex)
{
	this->entities.erase(this->entities.begin() + selectedIndex);
}

void Map::process_input(InputManager& inputManager, float deltaTime)
{
	//create debug menu

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

	if (inputManager.Keys[GLFW_KEY_M] && !inputManager.KeysProcessed[GLFW_KEY_M])
	{
		debugMenu.toggle_menu();
		inputManager.KeysProcessed[GLFW_KEY_M] = true;
	}

	// move to menu mode eventually
	if (debugMenu.is_menu_open())
	{
		if (inputManager.MouseButtons[GLFW_MOUSE_BUTTON_LEFT])
		{
			//essentially the distance function squares the difference of each vertice(xyz) adds them up and returns the sqrroot.
			//sqrt((objectcoord.x - objectpos.x) * *2 + (objectcoord.y - objectpos.y) * *2 + (objectcoord.z - objectpos.z)**2);
			//std::cout << "object cooords:" << min.x << ", " << min.y << ", " << min.z << ", " << std::endl;

			//color picking lol. Slow but not noticeable especially since it's used for map building.

			debugMenu.select_object(this->entities, this->camera, inputManager.xpos, inputManager.ypos);
		}

		if (inputManager.MouseButtons[GLFW_MOUSE_BUTTON_RIGHT])
		{
			debugMenu.deselect_index();
		}

		if (debugMenu.get_selected_index() != -1)
		{
			if (inputManager.Keys[GLFW_KEY_UP])
			{
				this->entities[debugMenu.get_selected_index()].Position.y += .03f;
			}
			if (inputManager.Keys[GLFW_KEY_DOWN])
			{
				this->entities[debugMenu.get_selected_index()].Position.y -= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_LEFT])
			{
				this->entities[debugMenu.get_selected_index()].Position.x += .03f;
			}
			if (inputManager.Keys[GLFW_KEY_RIGHT])
			{
				this->entities[debugMenu.get_selected_index()].Position.x -= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_J])
			{
				this->entities[debugMenu.get_selected_index()].Position.z += .03f;
			}
			if (inputManager.Keys[GLFW_KEY_K])
			{
				this->entities[debugMenu.get_selected_index()].Position.z -= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_N] && !inputManager.KeysProcessed[GLFW_KEY_N])
			{
				this->duplicate_model(debugMenu.get_selected_index());
				inputManager.KeysProcessed[GLFW_KEY_N] = true;
			}
			if (inputManager.Keys[GLFW_KEY_R] && !inputManager.KeysProcessed[GLFW_KEY_R])
			{
				this->remove_model(debugMenu.get_selected_index());
				inputManager.KeysProcessed[GLFW_KEY_R] = true;
			}
		}
	}
}
