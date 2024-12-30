#include"AssetManager.h"

std::map<unsigned int, Model*> assetMap;
SkyBox* skyBox;

void load_sky_box()
{
	std::vector<std::string> faces
	{
		"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/textures/skybox/right.jpg",
		"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/textures/skybox/left.jpg",
		"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/textures/skybox/top.jpg",
		"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/textures/skybox/bottom.jpg",
		"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/textures/skybox/front.jpg",
		"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/textures/skybox/back.jpg",
	};
	skyBox = new SkyBox(faces);
}

void AssetManager::load()
{
	assetMap[0] = new Model((char*)"C:/Dev/assets/knight/knight_skinned_0/knight_impl1.dae");
//	assetMap[0] = new Model((char*)"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/objects/vampire/dancing_vampire.dae");
	assetMap[1] = new Model((char*)"C:/Dev/assets/cube/cube.glb");
	assetMap[2] = new Model((char*)"C:/Dev/assets/plane/plane.obj");
	assetMap[3] = new Model((char*)"C:/Dev/assets/die.obj");
	assetMap[4] = new Model((char*)"C:/Dev/assets/step_platform/step_platform.obj");
	assetMap[5] = new Model((char*)"C:/Dev/assets/ball/ball.obj");
	assetMap[6] = new Model((char*)"C:/Dev/assets/goal.obj");

	load_sky_box();
}

Model* AssetManager::get_model(unsigned int id)
{
	return assetMap[id];
}

SkyBox* AssetManager::get_sky_box()
{
	return skyBox;
}