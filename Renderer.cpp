#include"Renderer.h"

UIManager* manager;

Shader* shaderProgram;
Shader* lightShaderProgram;
Shader* skinnedShaderProgram;
Shader* pickingShaderProgram;
Shader* cubemapShaderProgram;
Shader* uiShaderProgram;

std::vector<Player*> m_players;
std::vector<GameObject*> m_entities, m_lights;

Camera* m_camera;
DebugMenu* debugMenu;

glm::mat4 view, projection;

int selectedIndex = -1;

void Renderer::init_render(GLFWwindow* window)
{
	shaderProgram = new Shader("default.vert.glsl", "default.frag.glsl");
	lightShaderProgram = new Shader("light.vert.glsl", "light.frag.glsl");
	skinnedShaderProgram = new Shader("skinned.vert.glsl", "skinned.frag.glsl");
	pickingShaderProgram = new Shader("picking.vert.glsl", "picking.frag.glsl");
	cubemapShaderProgram = new Shader("cubemap.vert.glsl", "cubemap.frag.glsl");
	uiShaderProgram = new Shader("interface.vert.glsl", "interface.frag.glsl");

	manager = new UIManager(uiShaderProgram, 1920, 1080);
	debugMenu = new DebugMenu(window);
}

void Renderer::render(std::vector<Player*> players, std::vector<GameObject*> entities, std::vector<GameObject*> lights, Camera* camera, SkyBox* skybox)
{
	m_players = players;
	m_entities = entities;
	m_lights = lights;
	m_camera = camera;

	view = camera->get_view_matrix();
	projection = camera->get_projection_matrix();

	//draw first for environment mapping
	cubemapShaderProgram->use();
	cubemapShaderProgram->setInt("skybox", 0);
	cubemapShaderProgram->setMat4("view", glm::mat4(glm::mat3(m_camera->get_view_matrix())));
	cubemapShaderProgram->setMat4("projection", projection);
	skybox->draw();

	skinnedShaderProgram->use();
	skinnedShaderProgram->setVec3("lightPos", lights[0]->Position);
	skinnedShaderProgram->setVec3("lightColor", glm::vec3(1.0f));
	skinnedShaderProgram->setInt("skybox", 0);
	for (Player* player : players)
	{
		draw_aabb(player->GameModel->getMinAABB(), player->GameModel->getMaxAABB());
		Renderer::draw_skinned(player->GameModel, player->Position, player->Size, player->Rotation, player->transforms);

		if (player->state == ACTIVE)
		{
			manager->load_elements(player->get_cards(), player->get_selected_card_index());
		}
	}

	shaderProgram->use();
	shaderProgram->setVec3("lightPos", lights[0]->Position);
	shaderProgram->setVec3("lightColor", glm::vec3(1.0f));
	for (int i = 0; i < entities.size(); i++)
	{
		shaderProgram->setBool("selected", i == Renderer::get_selected_index());
		shaderProgram->setInt("objectId", entities[i]->id);
		Renderer::draw_static(shaderProgram, entities[i]->GameModel, entities[i]->Position, entities[i]->Size, entities[i]->Rotation);
	}

	lightShaderProgram->use();
	lightShaderProgram->setVec3("lightColor", glm::vec3(1.0f));
	for (GameObject* light : lights)
	{
		Renderer::draw_static(lightShaderProgram, light->GameModel, light->Position, light->Size, light->Rotation);
	}

	//this logic might need to be moved out and add setting the render items in the game logic. GOOD NOTE FOLLOW UP
	for (Player* player : players)
	{
		if (player->state == ACTIVE)
		{
			if (player->inMotion) {
				unsigned int card[1] = { player->get_cards()[player->get_selected_card_index()] };

				manager->load_elements(card, 6);
				manager->draw();
			}
			manager->draw();
		}
	}
}

void Renderer::draw_skinned(Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, std::vector<glm::mat4>* transform)
{
	skinnedShaderProgram->setMat4("view", view);
	skinnedShaderProgram->setMat4("projection", projection);

	for (int i = 0; i < transform->size(); ++i)
	{
		skinnedShaderProgram->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transform->at(i));
	}

	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), pos);

	modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat = glm::scale(modelMat, size);

	skinnedShaderProgram->setMat4("model", modelMat);

	model->draw(*skinnedShaderProgram);
}

void Renderer::draw_static(Shader* shader, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation)
{
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), pos);
	
	modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat = glm::scale(modelMat, size);

	shader->setMat4("model", modelMat);
	model->draw(*shaderProgram);
}

void Renderer::create_menu(float deltaTime)
{
	debugMenu->create_menu(m_entities, m_camera, deltaTime);
}

void Renderer::create_menu(float deltaTime, int roll)
{
	debugMenu->create_menu(m_entities, m_camera, deltaTime, roll);
}

void Renderer::select_entity(float xpos, float ypos)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pickingShaderProgram->use();

	pickingShaderProgram->setMat4("view", view);
	pickingShaderProgram->setMat4("projection", projection);

	for (int i = 0; i < m_entities.size(); i++)
	{
		int r = (i & 0x000000FF) >> 0;
		int g = (i & 0x0000FF00) >> 8;
		int b = (i & 0x00FF0000) >> 16;

		pickingShaderProgram->setVec4("PickingColor", glm::vec4((float)r / 255.0, (float)g / 255.0, (float)b / 255.0, 1.0f));
		Renderer::draw_static(pickingShaderProgram, m_entities[i]->GameModel, m_entities[i]->Position, m_entities[i]->Size, m_entities[i]->Rotation);
		glGetError();
	}

	glDisableVertexAttribArray(0);
	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char data[4];
	glReadPixels(xpos, 1080 - ypos - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	int pickedID = data[0] + (data[1] * 256) + (data[2] * 256 * 256);

	if (!(pickedID == 0x00ffffff))
	{
		std::ostringstream oss;
		oss << "selected mesh id: " << pickedID;
		std::cout << oss.str() << std::endl;

		selectedIndex = pickedID;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int Renderer::get_selected_index()
{
	return selectedIndex;
}

void Renderer::deselect_index()
{
	selectedIndex = -1;
}

void Renderer::draw_aabb(const glm::vec3& minAABB,const glm::vec3& maxAABB)
{
	const GLfloat vertices[] = {
		minAABB.x, minAABB.y, minAABB.z, 
		minAABB.x, minAABB.y, maxAABB.z, 
		minAABB.x, maxAABB.y, minAABB.z, 
		minAABB.x, maxAABB.y, maxAABB.z, 
		maxAABB.x, minAABB.y, minAABB.z, 
		maxAABB.x, minAABB.y, maxAABB.z, 
		maxAABB.x, maxAABB.y, minAABB.z, 
		maxAABB.x, maxAABB.y,maxAABB.z  
	};

	const GLuint indices[] = {
		0, 1, 1, 3, 3, 2, 2, 0,
		4, 5, 5, 7, 7, 6, 6, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}