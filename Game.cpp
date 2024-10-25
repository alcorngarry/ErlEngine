#include "Game.h"

Renderer *lightRenderer;
Renderer *modelRenderer;
Renderer *pickingRenderer;
GLFWwindow* gameWindow;

AssetManager assetManager;

Camera camera;
//GameObject* testFloor;
//GameObject* lightCube;

Player* player1;
Player* player2;
Player* player3;
Player* player4;

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

	assetManager = AssetManager();
	assetManager.load();

	camera = Camera();

	/*testFloor = new GameObject(*assetManager.get_model(2), glm::vec3(0.0f, -0.005f, 0.0f), glm::vec3(1000.0f));
	lightCube = new GameObject(*assetManager.get_model(1), glm::vec3(5.0f, 10.0f, 0.0f), glm::vec3(1.0f));*/

	this->Maps.push_back(Map("test_map_1"));
	Maps[0].load(assetManager);

	player1 = new Player(*assetManager.get_model(0), Maps[0].get_board_positions()[0] * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));
	player2 = new Player(*assetManager.get_model(0), Maps[0].get_board_positions()[0] * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));
	player3 = new Player(*assetManager.get_model(0), Maps[0].get_board_positions()[0] * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));
	player4 = new Player(*assetManager.get_model(0), Maps[0].get_board_positions()[0] * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));
}

void Game::update(float deltaTime)
{
	if (player1->inMotion)
	{	
		player1->move_player(Maps[0].get_board_positions());
	/*	camera.setCameraPos(player1->Position + glm::vec3(0.0f, 50.0f, 150.0f));
		camera.setCameraFront(player1->Position);*/
	}
	if (player2->inMotion)
	{
		player2->move_player(Maps[0].get_board_positions());
		//camera.setCameraPos(player2->Position + glm::vec3(0.0f, 30.0f, 50.0f));
		//camera.setCameraFront(player2->Position);
	}
	if (player3->inMotion)
	{
		player3->move_player(Maps[0].get_board_positions());
		//camera.setCameraPos(player3->Position + glm::vec3(0.0f, 30.0f, 50.0f));
		//camera.setCameraFront(player3->Position);
	}
	if (player4->inMotion)
	{
		player4->move_player(Maps[0].get_board_positions());
		//camera.setCameraPos(player4->Position + glm::vec3(0.0f, 30.0f, 50.0f));
		//camera.setCameraFront(player4->Position * 5.0f);
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

		if (this->Keys[GLFW_KEY_I])
		{
			if (player2->inMotion == false)
			{
				player2->inMotion = true;
				player2->start_move((float)glfwGetTime(), roll_dice() + 1);
			}
		}

		if (this->Keys[GLFW_KEY_O])
		{
			if (player3->inMotion == false)
			{
				player3->inMotion = true;
				player3->start_move((float)glfwGetTime(), roll_dice() + 1);
			}
		}

		if (this->Keys[GLFW_KEY_P])
		{
			if (player4->inMotion == false)
			{
				player4->inMotion = true;
				player4->start_move((float)glfwGetTime(), roll_dice() + 1);
			}
		}

		// move to menu mode eventually
		if (selectedIndex != -1)
		{
			if (this->Keys[GLFW_KEY_UP])
			{
				this->Maps[0].entities[selectedIndex].Position.y += .01f;
			}
			if (this->Keys[GLFW_KEY_DOWN])
			{
				this->Maps[0].entities[selectedIndex].Position.y -= .01f;
			}
			if (this->Keys[GLFW_KEY_LEFT])
			{
				this->Maps[0].entities[selectedIndex].Position.x += .01f;
			}
			if (this->Keys[GLFW_KEY_RIGHT])
			{
				this->Maps[0].entities[selectedIndex].Position.x -= .01f;
			}
			if (this->Keys[GLFW_KEY_J])
			{
				this->Maps[0].entities[selectedIndex].Position.z += .01f;
			}
			if (this->Keys[GLFW_KEY_K])
			{
				this->Maps[0].entities[selectedIndex].Position.z -= .01f;
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
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
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
		player2->draw(*modelRenderer);
		player3->draw(*modelRenderer);
		player4->draw(*modelRenderer);

		//testFloor->draw(*modelRenderer);
		this->Maps[0].draw(*modelRenderer, selectedIndex, false);

		lightRenderer->shader.use();
		glUniformMatrix4fv(glGetUniformLocation(lightRenderer->shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(lightRenderer->shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		lightRenderer->shader.setVec3("lightColor", glm::vec3(1.0f));
		//lightCube->draw(*lightRenderer);
		this->Maps[0].draw(*lightRenderer, selectedIndex, true);
	}
}

float Game::roll_dice()
{
	srand(time(0));
	int roll = rand() % 6;

	std::cout << "rolled " << roll + 1 << std::endl;
	return roll;
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


