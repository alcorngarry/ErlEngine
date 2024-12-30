#include"Map.h"

bool screenLock = false;

Map::Map(std::string mapName) : State(MENU_CLOSED)
{
	fileName = mapName;
	camera = Camera();
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

void Map::load()
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
			this->lights.push_back(new GameObject(id, AssetManager::get_model(id), glm::vec3(x, y, z), glm::vec3(sizeX, sizeY, sizeZ), glm::vec3(rotationX, rotationY, rotationZ)));
		}
		else
		{
			entities.push_back(new GameObject(id, AssetManager::get_model(id), glm::vec3(x, y, z), glm::vec3(sizeX, sizeY, sizeZ), glm::vec3(rotationX, rotationY, rotationZ)));
		} 
	} 
	readMap.close();

	load_players();
	load_skybox();
}

void Map::load_players()
{
	for (int i = 0; i < 4; i++)
	{
		players.push_back(new Player(AssetManager::get_model(0), glm::vec3(0.0f) * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f), glm::vec3(0.0f)));
	}
}

void Map::load_skybox()
{
	skybox = AssetManager::get_sky_box();
}

void Map::draw(float deltaTime)
{
	Renderer::render(players, entities, lights, camera, skybox);

	if (State == MENU_OPEN)
	{
		Renderer::create_menu(deltaTime);
	}
}

void Map::duplicate_model(int selectedIndex)
{
	entities.push_back(new GameObject(*entities[selectedIndex]));
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
			//debugMenu.select_object(entities, this->camera, inputManager.xpos, inputManager.ypos
			//check_intersection(inputManager.xpos, inputManager.ypos);
		}

		if (inputManager.MouseButtons[GLFW_MOUSE_BUTTON_RIGHT])
		{
			Renderer::deselect_index();
		}

		int selectedIndex = Renderer::get_selected_index();
		if (selectedIndex != -1)
		{

				// Calculate mouse delta (difference from last position)
				float deltaX = inputManager.xpos - inputManager.lastX;
				float deltaY = inputManager.ypos - inputManager.lastY;

				// Convert the delta to world space (this depends on your camera setup, this is a basic example)
				float moveSpeed = 1.0f; // You can adjust this speed based on your needs
				glm::vec3 movement = glm::vec3(deltaX * moveSpeed, -deltaY * moveSpeed, 0.0f);

				// Update the object's position
				entities[selectedIndex]->Position += movement;
			
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

//bool Map::AABBRayIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& min, const glm::vec3& max, float& tmin, float& tmax) {
//	tmin = 0.0f;
//	tmax = std::numeric_limits<float>::max();
// 
//	std::cout << "AABB Min: " << min.x << ", " << min.y << ", " << min.z << std::endl;
//	std::cout << "AABB Max: " << max.x << ", " << max.y << ", " << max.z << std::endl;
//
//	// Loop over all three axes (X, Y, Z)
//	for (int i = 0; i < 3; ++i) {
//		float origin = rayOrigin[i];
//		float direction = rayDir[i];
//		float boxMin = min[i];
//		float boxMax = max[i];
//
//		// Calculate tmin and tmax for each axis
//		float t1 = (boxMin - origin) / direction;
//		float t2 = (boxMax - origin) / direction;
//
//		// Ensure t1 is always the smaller value
//		if (t1 > t2) {
//			std::swap(t1, t2);
//		}
//
//		// Update the global tmin and tmax based on the current axis' intersection
//		tmin = std::max(tmin, t1);
//		tmax = std::min(tmax, t2);
//
//		// If the tmin > tmax at any point, there's no intersection
//		if (tmin > tmax) {
//			return false;
//		}
//	}
//
//	return true;
//}
//
//void Map::check_intersection(float x, float y)
//{
//	float tmin, tmax;
//	bool hit = false;
//
//	//create mouse ray
//	// ///////////////////////////////////
//	 // Get the window's dimensions (optional: make sure you use the correct values for width and height)
//	glm::vec4 viewport = glm::vec4(0, 0, 1920, 1080);  // Update with actual window size
//
//	// Calculate the inverse projection and view matrices
//	glm::mat4 inverseProjection = glm::inverse(camera.get_projection_matrix());
//	glm::mat4 inverseView = glm::inverse(camera.get_view_matrix());
//
//	// Map the mouse position (x, y) to normalized device coordinates (NDC)
//	glm::vec3 near_point = glm::unProject(glm::vec3(x, y, 0.0f), inverseView, inverseProjection, viewport);
//	glm::vec3 far_point = glm::unProject(glm::vec3(x, y, 1.0f), inverseView, inverseProjection, viewport);
//
//	// Create the ray direction from the near_point to the far_point
//	glm::vec3 ray_dir = glm::normalize(far_point - near_point);
//	////////////////////////////////////////
//
//	std::cout << "Ray origin: " << near_point.x << ", " << near_point.y << ", " << near_point.z << std::endl;
//	std::cout << "Ray direction: " << ray_dir.x << ", " << ray_dir.y << ", " << ray_dir.z << std::endl;
//
//
//	// Iterate over all game objects and check for intersection
//	for (const auto& gameObject : entities) {
//		// Get the AABB for the current GameObject
//
//		// Test for intersection with the AABB
//		if (AABBRayIntersection(near_point, ray_dir, gameObject->get_aabb_min(), gameObject->get_aabb_max(), tmin, tmax)) {
//			std::cout << "Ray intersects with GameObject at tmin = " << tmin << " and tmax = " << tmax << std::endl;
//			hit = true;
//		}
//	}
//
//	if (!hit) {
//		std::cout << "Ray did not intersect with any GameObject." << std::endl;
//	}
//}
