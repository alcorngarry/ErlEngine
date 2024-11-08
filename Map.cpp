#include"Map.h"

bool screenLock = false;

Map::Map(std::string mapName, DebugMenu debugMenu) : State(MENU_CLOSED)
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
		writeMap << this->entities.at(i).id << ", "
			<< this->entities.at(i).Position.x << "," << this->entities.at(i).Position.y << "," << this->entities.at(i).Position.z  << ", "
			<< this->entities.at(i).Size.x << "," << this->entities.at(i).Size.y << "," << this->entities.at(i).Size.z << ", "
			<< this->entities.at(i).Rotation.x << "," << this->entities.at(i).Rotation.y << "," << this->entities.at(i).Rotation.z << std::endl;
	}

	for (GameObject light : lights)
	{
		writeMap << light.id << ", "
			<< light.Position.x << "," << light.Position.y << "," << light.Position.z << ", "
			<< light.Size.x << "," << light.Size.y << "," << light.Size.z << ", "
			<< light.Rotation.x << "," << light.Rotation.y << "," << light.Rotation.z << std::endl;
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
			this->lights.push_back(GameObject(id, *assetManager.get_model(id), glm::vec3(x, y, z), glm::vec3(sizeX, sizeY, sizeZ), glm::vec3(rotationX, rotationY, rotationZ)));
		}
		else if (id == 5)
		{
			this->entities.push_back(GameObject(id, *assetManager.get_model(id), glm::vec3(x, y, z), glm::vec3(sizeX, sizeY, sizeZ), glm::vec3(rotationX, rotationY, rotationZ)));
			ballVal = entities.size() - 1;
		}
		else
		{
			this->entities.push_back(GameObject(id, *assetManager.get_model(id), glm::vec3(x, y, z), glm::vec3(sizeX, sizeY, sizeZ), glm::vec3(rotationX, rotationY, rotationZ)));
		} 
	} 
	readMap.close();

	load_players(assetManager);
}

void Map::load_players(AssetManager assetManager)
{
	player1 = new Player(*assetManager.get_model(0), glm::vec3(0.0f) * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));
	player2 = new Player(*assetManager.get_model(0), glm::vec3(0.0f) * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));
	player3 = new Player(*assetManager.get_model(0), glm::vec3(0.0f) * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));
	player4 = new Player(*assetManager.get_model(0), glm::vec3(0.0f) * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));
}


void Map::draw(Renderer& renderer, bool isLight, float deltaTime)
{
	if (State == MENU_OPEN)
	{
		debugMenu.create_menu(this->entities, this->camera, deltaTime);
	}
	for (int i = 0; i < this->entities.size(); i++)
	{
		renderer.shader.setBool("selected", i == debugMenu.get_selected_index());
		this->entities.at(i).draw(renderer);
	}

	if (isLight)
	{
		for (int i = 0; i < this->lights.size(); i++)
		{
			this->lights.at(i).draw(renderer);
		}
	}
	else {
		player1->draw(renderer);
		player2->draw(renderer);
		player3->draw(renderer);
		player4->draw(renderer);

		player1->draw_bounding_box(renderer);
		player2->draw_bounding_box(renderer);
		player3->draw_bounding_box(renderer);
		player4->draw_bounding_box(renderer);
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

			if (inputManager.Keys[GLFW_KEY_R] && !inputManager.KeysProcessed[GLFW_KEY_R])
			{
				this->remove_model(debugMenu.get_selected_index());
				inputManager.KeysProcessed[GLFW_KEY_R] = true;
			}

			if (inputManager.Keys[GLFW_KEY_1])
			{
				this->entities[debugMenu.get_selected_index()].Size.y *= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_2])
			{
				this->entities[debugMenu.get_selected_index()].Size.y *= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_3])
			{
				this->entities[debugMenu.get_selected_index()].Size.x *= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_4])
			{
				this->entities[debugMenu.get_selected_index()].Size.x *= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_5])
			{
				this->entities[debugMenu.get_selected_index()].Size.z *= .03f;
			}
			if (inputManager.Keys[GLFW_KEY_6])
			{
				this->entities[debugMenu.get_selected_index()].Size.z *= .03f;
			}
		}
	}
}