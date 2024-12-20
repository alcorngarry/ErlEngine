#include"AssetManager.h"

AssetManager::AssetManager()
{
}

void AssetManager::load()
{
	//this->assetMap[0] = new Model((char*)"C:/Dev/assets/knight/knight_impl1.obj");
	assetMap[0] = new Model((char*)"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/objects/vampire/dancing_vampire.dae");
	assetMap[1] = new Model((char*)"C:/Dev/assets/cube/cube.glb");
	assetMap[2] = new Model((char*)"C:/Dev/assets/plane/plane.obj");
	assetMap[3] = new Model((char*)"C:/Dev/assets/die.obj");
	assetMap[4] = new Model((char*)"C:/Dev/assets/step_platform/step_platform.obj");
	assetMap[5] = new Model((char*)"C:/Dev/assets/ball/ball.obj");
	assetMap[6] = new Model((char*)"C:/Dev/assets/goal.obj");
}

Model* AssetManager::get_model(unsigned int id)
{
	return assetMap[id];
}