#include"Map.h"


Map::Map(std::string mapName)
{
	fileName = mapName;
}

void Map::save()
{
	writeMap = std::ofstream{ fileName + ".txt" };
	for (int i = 0; i < this->entities.size(); i++)
	{
		writeMap << "4, " << this->entities.at(i).Position.x << "," << this->entities.at(i).Position.y << "," << this->entities.at(i).Position.z << std::endl;
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
			//this->boardPositions.push_back(glm::vec3(x, y, z));
			GameObject platForm(*assetManager.get_model(id), glm::vec3(x, y, z), glm::vec3(10.0f));
			this->entities.push_back(platForm);
		}
	}
	//if (this->boardPositions.empty())
	//{
	//	this->boardPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	//} 
	readMap.close();
}

void Map::draw(Renderer &renderer, int selectedIndex)
{
	for (int i = 0; i < this->entities.size(); i++)
	{
		renderer.shader.setBool("selected", i == selectedIndex);
		this->entities.at(i).draw(renderer);
	}
}

void Map::draw_picking(Renderer& renderer)
{
	for (int i = 0; i < this->entities.size(); i++)
	{
		int r = (i & 0x000000FF) >> 0;
		int g = (i & 0x0000FF00) >> 8;
		int b = (i & 0x00FF0000) >> 16;

		renderer.shader.setVec4("PickingColor", glm::vec4((float)r / 255.0, (float)g / 255.0, (float)b / 255.0, 1.0f));
		this->entities.at(i).draw(renderer);
	}
}

void Map::duplicate_model(int selectedIndex)
{
	this->entities.push_back(entities[selectedIndex]);
	//this->boardPositions.push_back(entities[entities.size() - 1].Position);
}

void Map::remove_model(int selectedIndex)
{
	
	this->entities.erase(this->entities.begin() + selectedIndex);
	//this->boardPositions.erase(this->boardPositions.begin() + selectedIndex);
}

std::vector<glm::vec3> Map::get_board_positions()
{
	std::vector<glm::vec3> output;

	for (GameObject entity: this->entities)
	{
		output.push_back(entity.Position);
	}
	return output;
}