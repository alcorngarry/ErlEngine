#pragma once
#include"Shader.h"
#include"Model.h"
#include"GameObject.h"
#include"Player.h"
#include"Camera.h"
#include"DebugMenu.h"


namespace Renderer {
	//Renderer(Shader &shader);
	//~Renderer();
	GLFWwindow* init_render();
	void create_renderer(std::vector<Player> players, std::vector<GameObject> entities, std::vector<GameObject> lights, Camera camera);

	//void draw(Model &model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
	void draw_entity(Model& model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
	void draw_light(Model& model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
	void draw_player(Model& model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, std::vector<glm::mat4> transform);
	void create_menu(float deltaTime);
	void deselect_index();
	int get_selected_index();
	void select_entity(float xpos, float ypos);
	//void draw(Model& model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, std::vector<glm::mat4> transform);
};
