#pragma once
#include"Shader.h"
#include"Model.h"
#include"GameObject.h"
#include"Player.h"
#include"Camera.h"
#include"DebugMenu.h"


namespace Renderer {
	GLFWwindow* init_render();
	void render(std::vector<Player*> players, std::vector<GameObject*> entities, std::vector<GameObject*> lights, Camera& camera);

	void create_menu(float deltaTime);
	void deselect_index();
	int get_selected_index();
	void select_entity(float xpos, float ypos);
	//private
	void draw_aabb(const glm::vec3& minAABB,const glm::vec3& maxAABB);
	void draw_static(Shader* shader, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
	void draw_skinned(Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, std::vector<glm::mat4>* transform);
};
