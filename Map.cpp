#include"Map.h"


Map::Map(std::string mapName)
{
	fileName = mapName;
}

void Map::save(std::vector<glm::vec3> objLoc)
{
	writeMap = std::ofstream{ fileName + ".txt" };
	for (int i = 0; i < objLoc.size(); i++)
	{
		writeMap << objLoc.at(i).x << "," << objLoc.at(i).y << "," << objLoc.at(i).z << std::endl;
	}
	writeMap.close();
}

void Map::load()
{
	readMap.open(fileName + ".txt");
	std::string line;
	float x, y, z;

	while (getline(readMap, line, ','))
	{
		//parse x
		x = std::stof(line);

		// Parse y
		getline(readMap, line, ',');
		y = std::stof(line);

		// Parse z
		getline(readMap, line);
		z = std::stof(line);

		// Return the constructed glm::vec3
		this->boardPositions.push_back(glm::vec3(x, y, z));
	}
	if (this->boardPositions.empty())
	{
		this->boardPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	} 
	readMap.close();

	Model plat((char*)"C:/Dev/assets/step_platform.obj");

	for (int i = 0; i < this->boardPositions.size(); i++)
	{
		std::cout << this->boardPositions.at(i).x << this->boardPositions.at(i).y << this->boardPositions.at(i).z << std::endl;
		GameObject platForm(plat, this->boardPositions.at(i), glm::vec3(10.0f));
		this->platForms.push_back(platForm);
	}
}

void Map::draw(Renderer &renderer, int selectedIndex)
{
	for (int i = 0; i < this->platForms.size(); i++)
	{
		renderer.shader.setBool("selected", i == selectedIndex);
		this->platForms.at(i).draw(renderer);
	}
}

void Map::draw_picking(Renderer& renderer)
{
	for (int i = 0; i < this->platForms.size(); i++)
	{
		int r = (i & 0x000000FF) >> 0;
		int g = (i & 0x0000FF00) >> 8;
		int b = (i & 0x00FF0000) >> 16;

		renderer.shader.setVec4("PickingColor", glm::vec4((float)r / 255.0, (float)g / 255.0, (float)b / 255.0, 1.0f));
		this->platForms.at(i).draw(renderer);
	}
}

void Map::duplicate_model(int selectedIndex)
{
	this->platForms.push_back(platForms[selectedIndex]);
	this->boardPositions.push_back(platForms[platForms.size() - 1].Position);
}

void Map::remove_model(int selectedIndex)
{
	
	this->platForms.erase(this->platForms.begin() + selectedIndex);
	this->boardPositions.erase(this->boardPositions.begin() + selectedIndex);
}

std::vector<glm::vec3> Map::get_board_positions()
{
	return this->boardPositions;
}