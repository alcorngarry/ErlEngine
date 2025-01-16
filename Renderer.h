#pragma once
#include"Shader.h"
#include"Model.h"
#include"GameObject.h"
#include"Player.h"
#include"Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "UIManager.h"
#include "DebugMenu.h"
#include "stb_image.h"
#include "AssetManager.h"
#include "UIElement.h"

namespace Renderer {
	void init_render(GLFWwindow* window);
	void render(std::vector<Player*> players, std::vector<GameObject*> entities, std::vector<GameObject*> lights, Camera* camera, SkyBox* skybox);

	void create_menu(float deltaTime);
	void create_menu(float deltaTime, int roll);
	void deselect_index();
	int get_selected_index();
	void select_entity(float xpos, float ypos);
	//private
	void draw_aabb(const glm::vec3& minAABB,const glm::vec3& maxAABB);
	void draw_static(Shader* shader, Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
	void draw_skinned(Model* model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, std::vector<glm::mat4>* transform);
};
