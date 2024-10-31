#include "Game.h"

Renderer *lightRenderer;
Renderer *modelRenderer;
GLFWwindow* gameWindow;

AssetManager assetManager;

DebugMenu debugMenu;

Player* player1;
Player* player2;
Player* player3;
Player* player4;

bool isWireFrame = false;
int selectedIndex = -1;

Game::Game(GLFWwindow* window) : State(GAME_ACTIVE)
{
	inputManager = InputManager(window);
}

Game::~Game()
{
	delete lightRenderer;
	delete player1;
	delete player2;
	delete player3;
	delete player4;
	delete modelRenderer;
}

void Game::init(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	gameWindow = window;

	Shader shaderProgram("default.vert.glsl", "default.frag.glsl");
	Shader lightShaderProgram("light.vert.glsl", "light.frag.glsl");
	
	lightRenderer = new Renderer(lightShaderProgram);
	modelRenderer = new Renderer(shaderProgram);

	assetManager = AssetManager();
	assetManager.load();

	this->Maps.push_back(BoardMap("test_map_1", debugMenu));
	this->Maps.push_back(PongMap("test_map_2", debugMenu));
	this->level = 1;

	Maps[level].load(assetManager);

	debugMenu = DebugMenu(gameWindow);

	/*player1 = new Player(*assetManager.get_model(0), (BoardMap)Maps[level].get_board_positions()[0] * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));
	player2 = new Player(*assetManager.get_model(0), (BoardMap)Maps[level].get_board_positions()[0] * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));
	player3 = new Player(*assetManager.get_model(0), (BoardMap)Maps[level].get_board_positions()[0] * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));
	player4 = new Player(*assetManager.get_model(0), (BoardMap)Maps[level].get_board_positions()[0] * 5.0f + glm::vec3(0, 3, 0), glm::vec3(2.0f));*/
}

void Game::update(float deltaTime)
{
	inputManager.Update();
	//if (player1->inMotion)
	//{	
	//	player1->move_player(Maps[level].get_board_positions());

	///*	camera.setCameraPos(player1->Position + glm::vec3(0.0f, 50.0f, 150.0f));
	//	camera.setCameraFront(player1->Position);*/
	//}
	//if (player2->inMotion)
	//{
	//	player2->move_player(Maps[level].get_board_positions());
	//	//camera.setCameraPos(player2->Position + glm::vec3(0.0f, 30.0f, 50.0f));
	//	//camera.setCameraFront(player2->Position);
	//}
	//if (player3->inMotion)
	//{
	//	player3->move_player(Maps[level].get_board_positions());
	//	//camera.setCameraPos(player3->Position + glm::vec3(0.0f, 30.0f, 50.0f));
	//	//camera.setCameraFront(player3->Position);
	//}
	//if (player4->inMotion)
	//{
	//	player4->move_player(Maps[level].get_board_positions());
	//	//camera.setCameraPos(player4->Position + glm::vec3(0.0f, 30.0f, 50.0f));
	//	//camera.setCameraFront(player4->Position * 5.0f);
	//}

	//if (!player1->inMotion && player1->get_board_position() > 6)
	//{
	//	this->level = 1;
	//	//Maps[level].load(assetManager);
	//}

	//Maps[level].get_ball().Position += glm::vec3(1.0f, 0.0f, 0.0f);
}

void Game::process_input(float deltaTime)
{
	Maps[level].process_input(inputManager, deltaTime);

	if (this->State == GAME_ACTIVE || this->State == DEBUG_MENU)
	{
		if (inputManager.Keys[GLFW_KEY_M] && !inputManager.KeysProcessed[GLFW_KEY_M])
		{
			if (this->State == DEBUG_MENU)
			{
				this->State = GAME_ACTIVE;
			}
			else {
				this->State = DEBUG_MENU;
			}
			this->inputManager.KeysProcessed[GLFW_KEY_M] = true;
		}

		if (this->State == DEBUG_MENU)
		{
			if (this->inputManager.Keys[GLFW_KEY_P])
			{
				if (isWireFrame)
				{
					isWireFrame = false;
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				else {
					isWireFrame = true;
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
			}
		}
	}
}

void Game::render(float deltaTime)
{
	/*if (this->State == GAME_ACTIVE)
	{	*/
		glm::mat4 view = glm::lookAt(Maps[level].camera.getCameraPos(), Maps[level].camera.getCameraPos() + Maps[level].camera.getCameraFront(), Maps[level].camera.getCameraUp());
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 10000.0f);
		
		modelRenderer->shader.use();
		glUniformMatrix4fv(glGetUniformLocation(modelRenderer->shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(modelRenderer->shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		modelRenderer->shader.setVec3("viewPos", Maps[level].camera.getCameraPos());
		modelRenderer->shader.setBool("selected", false);
		modelRenderer->shader.setVec3("lightPos", glm::vec3(5.0f, 0.0f, 0.0f));
		modelRenderer->shader.setVec3("lightColor", glm::vec3(1.0f));
		
		/*player1->draw(*modelRenderer);
		player2->draw(*modelRenderer);
		player3->draw(*modelRenderer);
		player4->draw(*modelRenderer);*/

		Maps[level].draw(*modelRenderer, false, deltaTime);

		lightRenderer->shader.use();
		glUniformMatrix4fv(glGetUniformLocation(lightRenderer->shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(lightRenderer->shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		lightRenderer->shader.setVec3("lightColor", glm::vec3(1.0f));
		Maps[level].draw(*lightRenderer, true, deltaTime);

		/*if (this->State == DEBUG_MENU)
		{
			debugMenu->create_menu(this->Maps[level], deltaTime);
		}*/
	//}
}


