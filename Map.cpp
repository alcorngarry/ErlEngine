#include"Map.h"

Map::Map(std::string mapName) : loadState(KEEP_MAP), state(DEFAULT)
{
	fileName = mapName;
	camera = new Camera();
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
	int id, boardSpaceId;

	while (getline(readMap, line, ','))
	{
		// Parse id
		id = std::stof(line);

		//check space type 0 : blank, 1 : add3, 2 add6, 3 remove3
		if (id == 4)
		{
			getline(readMap, line, ',');
			boardSpaceId = std::stof(line);
		}

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

		//if light
		if (id == 1)
		{
			//set Id to 10 right now to avoid collisions with board space Idds
			this->lights.push_back(new GameObject(10, AssetManager::get_model(id), glm::vec3(x, y, z), glm::vec3(sizeX, sizeY, sizeZ), glm::vec3(rotationX, rotationY, rotationZ)));
		}
		//if board space
		else if (id == 4)
		{
			entities.push_back(new GameObject(boardSpaceId, AssetManager::get_model(id), glm::vec3(x, y, z), glm::vec3(sizeX, sizeY, sizeZ), glm::vec3(rotationX, rotationY, rotationZ)));
		}
		else
		{
			entities.push_back(new GameObject(11, AssetManager::get_model(id), glm::vec3(x, y, z), glm::vec3(sizeX, sizeY, sizeZ), glm::vec3(rotationX, rotationY, rotationZ)));
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
		players.push_back(new Player(i, AssetManager::get_model(0), glm::vec3(0.0f) * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f), glm::vec3(0.0f), playerControls));
	}
}

void Map::load_skybox()
{
	skybox = AssetManager::get_sky_box();
}

void Map::draw(float deltaTime)
{
	Renderer::render(players, entities, lights, camera, skybox);
}

void Map::duplicate_model(int selectedIndex)
{
	entities.push_back(new GameObject(*entities[selectedIndex]));
}

void Map::remove_model(int selectedIndex)
{
	entities.erase(entities.begin() + selectedIndex);
}