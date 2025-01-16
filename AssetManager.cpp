#include"AssetManager.h"

std::map<unsigned int, Model*> assetMap;
SkyBox* skyBox;
unsigned int cardTextures[13];

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

unsigned int load_texture(const char* filePath) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(0);
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

	if (data) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		//mipmap requires power of two sizing that's why there was an issue, look into mipmapping
		//glGenerateMipmap(GL_TEXTURE_2D);

		std::cout << "Loaded texture: " << filePath << std::endl;
	}
	else {
		std::cout << "Failed to load texture: " << filePath << std::endl;
	}
	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureID;
}

void AssetManager::load()
{
	assetMap[0] = new Model((char*)"C:/Dev/assets/knight/knight_skinned_0/knight_impl1.dae");
	assetMap[1] = new Model((char*)"C:/Dev/assets/cube/cube.glb");
	assetMap[2] = new Model((char*)"C:/Dev/assets/plane/plane.obj");
	assetMap[3] = new Model((char*)"C:/Dev/assets/die.obj");
	assetMap[4] = new Model((char*)"C:/Dev/assets/step_platform/step_platform.obj");
	assetMap[5] = new Model((char*)"C:/Dev/assets/ball/ball.obj");
	assetMap[6] = new Model((char*)"C:/Dev/assets/goal.obj");
	assetMap[7] = new Model((char*)"C:/Dev/assets/GameCube - Mario Party 7 - Sheep/Sheep/sheep.fbx");

	load_sky_box();

	cardTextures[0] = load_texture("C:/Dev/assets/cards/c01.png");
	cardTextures[1] = load_texture("C:/Dev/assets/cards/c02.png");
	cardTextures[2] = load_texture("C:/Dev/assets/cards/c03.png");
	cardTextures[3] = load_texture("C:/Dev/assets/cards/c04.png");
	cardTextures[4] = load_texture("C:/Dev/assets/cards/c05.png");
	cardTextures[5] = load_texture("C:/Dev/assets/cards/c06.png");
	cardTextures[6] = load_texture("C:/Dev/assets/cards/c07.png");
	cardTextures[7] = load_texture("C:/Dev/assets/cards/c08.png");
	cardTextures[8] = load_texture("C:/Dev/assets/cards/c09.png");
	cardTextures[9] = load_texture("C:/Dev/assets/cards/c10.png");
	cardTextures[10] = load_texture("C:/Dev/assets/cards/c11.png");
	cardTextures[11] = load_texture("C:/Dev/assets/cards/c12.png");
	cardTextures[12] = load_texture("C:/Dev/assets/cards/c13.png");
}

Model* AssetManager::get_model(unsigned int id)
{
	return assetMap[id];
}

SkyBox* AssetManager::get_sky_box()
{
	return skyBox;
}

unsigned int AssetManager::get_card(unsigned int card)
{
	return cardTextures[card];
}