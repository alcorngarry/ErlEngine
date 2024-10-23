#include "Game.h"

Renderer *lightRenderer;
Renderer *modelRenderer;
Renderer *pickingRenderer;
GLFWwindow* gameWindow;

Model *backpack;
Model *cube;
Model *mapFloor;
Model *die;

Camera camera;
GameObject* testFloor;
GameObject* lightCube;

Player* player1;

int selectedIndex = -1;

Game::Game() : State(GAME_ACTIVE)
{
}

Game::~Game()
{
}

void Game::init(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	gameWindow = window;

	Shader shaderProgram("default.vert.glsl", "default.frag.glsl");
	Shader lightShaderProgram("light.vert.glsl", "light.frag.glsl");
	Shader pickingShaderProgram("picking.vert.glsl", "picking.frag.glsl");
	
	lightRenderer = new Renderer(lightShaderProgram);
	modelRenderer = new Renderer(shaderProgram);
	pickingRenderer = new Renderer(pickingShaderProgram);

	backpack = new Model((char*)"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/objects/backpack/backpack.obj");
	cube = new Model((char*)"C:/Dev/assets/cube.glb");
	mapFloor = new Model((char*)"C:/Dev/assets/plane/plane.obj");
	die = new Model((char*)"C:/Dev/assets/die.obj");
	
	camera = Camera();

	testFloor = new GameObject(Model((char*)"C:/Dev/assets/plane/plane.obj"), glm::vec3(0.0f, -0.005f, 0.0f), glm::vec3(1000.0f));
	lightCube = new GameObject(Model((char*)"C:/Dev/assets/cube.glb"), glm::vec3(5.0f, 10.0f, 0.0f), glm::vec3(1.0f));

	this->Maps.push_back(Map("test_map_1"));
	Maps[0].load();

	player1 = new Player(*backpack, Maps[0].get_board_positions()[0] * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));
}

void Game::update(float deltaTime)
{
	if (player1->inMotion)
	{	
		player1->move_player(Maps[0].get_board_positions());
	}
}

void Game::process_input(float deltaTime, float yaw, float pitch, float xpos, float ypos)
{
	if (this->State == GAME_ACTIVE)
	{
		camera.setCameraFront(glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch)))));

		float cameraSpeed = static_cast<float>(150 * deltaTime);

		if (this->MouseButtons[GLFW_MOUSE_BUTTON_LEFT])
		{
			//essentially the distance function squares the difference of each vertice(xyz) adds them up and returns the sqrroot.
			//sqrt((objectcoord.x - objectpos.x) * *2 + (objectcoord.y - objectpos.y) * *2 + (objectcoord.z - objectpos.z)**2);
			//std::cout << "object cooords:" << min.x << ", " << min.y << ", " << min.z << ", " << std::endl;

			//color picking lol. Slow but not noticeable especially since it's used for map building.

			colorPicking(xpos, ypos);
		}

		if (this->MouseButtons[GLFW_MOUSE_BUTTON_RIGHT])
		{
			selectedIndex = -1;
		}

		if (this->Keys[GLFW_KEY_W])
		{
			camera.setCameraPos(camera.getCameraPos() + cameraSpeed * camera.getCameraFront());
		}
		if (this->Keys[GLFW_KEY_A])
		{
			camera.setCameraPos(camera.getCameraPos() - glm::normalize(glm::cross(camera.getCameraFront(), camera.getCameraUp())) * cameraSpeed);

		}
		if (this->Keys[GLFW_KEY_S])
		{
			camera.setCameraPos(camera.getCameraPos() - cameraSpeed * camera.getCameraFront());

		}
		if (this->Keys[GLFW_KEY_D])
		{
			camera.setCameraPos(camera.getCameraPos() + glm::normalize(glm::cross(camera.getCameraFront(), camera.getCameraUp())) * cameraSpeed);

		}
		if (this->Keys[GLFW_KEY_SPACE])
		{
			if (player1->inMotion == false)
			{
				player1->inMotion = true;
				player1->start_move((float)glfwGetTime(), roll_dice() + 1);
			}
		}

		// move to menu mode eventually
		if (selectedIndex != -1)
		{
			if (this->Keys[GLFW_KEY_UP])
			{
				this->Maps[0].platForms[selectedIndex].Position.y += .01f;
			}
			if (this->Keys[GLFW_KEY_DOWN])
			{
				this->Maps[0].platForms[selectedIndex].Position.y -= .01f;
			}
			if (this->Keys[GLFW_KEY_LEFT])
			{
				this->Maps[0].platForms[selectedIndex].Position.x += .01f;
			}
			if (this->Keys[GLFW_KEY_RIGHT])
			{
				this->Maps[0].platForms[selectedIndex].Position.x -= .01f;
			}
			if (this->Keys[GLFW_KEY_J])
			{
				this->Maps[0].platForms[selectedIndex].Position.z += .01f;
			}
			if (this->Keys[GLFW_KEY_K])
			{
				this->Maps[0].platForms[selectedIndex].Position.z -= .01f;
			}
			if (this->Keys[GLFW_KEY_N] && !this->KeysProcessed[GLFW_KEY_N])
			{
				this->Maps[0].duplicate_model(selectedIndex);
				this->KeysProcessed[GLFW_KEY_N] = true;
			}
			if (this->Keys[GLFW_KEY_R] && !this->KeysProcessed[GLFW_KEY_R])
			{
				this->Maps[0].remove_model(selectedIndex);
				this->KeysProcessed[GLFW_KEY_R] = true;
			}
		}
	}
}

void Game::render()
{
	
	if (this->State == GAME_ACTIVE)
	{	
		glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 10000.0f);
		
		modelRenderer->shader.use();
		glUniformMatrix4fv(glGetUniformLocation(modelRenderer->shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(modelRenderer->shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		modelRenderer->shader.setVec3("viewPos", camera.getCameraPos());
		modelRenderer->shader.setBool("selected", false);
		modelRenderer->shader.setVec3("lightPos", glm::vec3(5.0f, 0.0f, 0.0f));
		modelRenderer->shader.setVec3("lightColor", glm::vec3(1.0f));
		
		player1->draw(*modelRenderer);
		testFloor->draw(*modelRenderer);
		this->Maps[0].draw(*modelRenderer, selectedIndex);

		lightRenderer->shader.use();
		glUniformMatrix4fv(glGetUniformLocation(lightRenderer->shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(lightRenderer->shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		lightRenderer->shader.setVec3("lightColor", glm::vec3(1.0f));
		lightCube->draw(*lightRenderer);
	}
}

float Game::roll_dice()
{
	srand(time(0));
	int roll = rand() % 6;

	if (roll == 0)
	{
		std::cout << "rolled " << 1 << std::endl;
		return roll;
	}
	else if (roll == 1)
	{
		std::cout << "rolled " << 2 << std::endl;
		return roll;

	}
	else if (roll == 2)
	{
		std::cout << "rolled " << 3 << std::endl;
		return roll;
	}
	else if (roll == 3)
	{
		std::cout << "rolled " << 4 << std::endl;
		return roll;

	}
	else if (roll == 4)
	{
		std::cout << "rolled " << 5 << std::endl;
		return roll;
	}
	else if (roll == 5)
	{
		std::cout << "rolled " << 6 << std::endl;
		return roll;
	}
}

void Game::colorPicking(float xpos, float ypos)
{	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//-------duplicate fix eventually
	glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 10000.0f);
	//
	pickingRenderer->shader.use();
	glEnableVertexAttribArray(0);
	glUniformMatrix4fv(glGetUniformLocation(pickingRenderer->shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(pickingRenderer->shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	this->Maps[0].draw_picking(*pickingRenderer);
	
	glDisableVertexAttribArray(0);
	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char data[4];
	glReadPixels(xpos, 768 - ypos - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	int pickedID = data[0] + (data[1] * 256) + (data[2] * 256 * 256);

	std::cout << xpos << ", " << ypos << std::endl;

	if (!(pickedID == 0x00ffffff))
	{
		std::ostringstream oss;
		oss << "mesh " << pickedID;
		std::cout << oss.str() << std::endl;

		selectedIndex = pickedID;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


