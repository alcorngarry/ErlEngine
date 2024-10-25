#include"AssetManager.h"

AssetManager::AssetManager()
{
}

void AssetManager::load()
{
	this->assetMap[0] = new Model((char*)"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/objects/backpack/backpack.obj");
	this->assetMap[1] = new Model((char*)"C:/Dev/assets/cube.glb");
	this->assetMap[2] = new Model((char*)"C:/Dev/assets/plane/plane.obj");
	this->assetMap[3] = new Model((char*)"C:/Dev/assets/die.obj");
	this->assetMap[4] = new Model((char*)"C:/Dev/assets/step_platform.obj");
}

Model* AssetManager::get_model(unsigned int id)
{
	return assetMap[id];
}