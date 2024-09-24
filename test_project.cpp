#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"Model.h"
#include"CharacterController.h"
#include"DebugMenu.h"
#include"Map.h"

#include<GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void get_resolution();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void toggleMenu(GLFWwindow* window);
void drawFacelessCube(Shader& shader, glm::vec3 min, glm::vec3 max, glm::mat4 model);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void duplicateModel();
void removeModel();
void selectObject(GLFWwindow* window, Shader shader, Model model);
void characterMove(Model character, Shader shader);
void diceRoll(Shader shader, Model die);

float roll_dice();

CharacterController controller;
Camera camera;

std::vector<glm::vec3> objectsInScene;

glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 ray_coord;
glm::vec3 ray_wor, front;
glm::vec3 out_direction;

glm::vec3 charStartPos;

glm::mat4 projection, view;


float deltaTime = 0.0f, lastFrame = 0.0f, rotationY = 0.0f, rotationX = 0.0f;
float window_width, window_height;

float xpos;
float ypos;

unsigned int texture1, texture2, selectedObjectIndex;

bool selected = false, firstMouse = true, isMenuOpen = false, isDiceRolled = false, preDiceRoll = true;

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = window_width / 2.0;
float lastY = window_height / 2.0;
float fov = 60.0f;
float roll = 0.0f;
float startTime = 0.0f;
int totalMoves = 0;
int startPos = 0;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	get_resolution();
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "test_game", NULL, NULL);
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader shaderProgram("default.vert.glsl", "default.frag.glsl");
	Shader lightShaderProgram("light.vert.glsl", "light.frag.glsl");
	Shader pickingShaderProgram("picking.vert.glsl", "picking.frag.glsl");

	Model backpack((char*)"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/objects/backpack/backpack.obj");
	//Model test((char*)"C:/Dev/assets/untitled.obj");
	
	Model cube((char*)"C:/Dev/assets/cube.glb");
	Model floor((char*)"C:/Dev/assets/plane/plane.obj");
	Model die((char*)"C:/Dev/assets/die.obj");
	Model platform((char*)"C:/Dev/assets/step_platform.obj");

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	camera = Camera();
	controller = CharacterController();
	DebugMenu debugMenu = DebugMenu(window);

	Map testMap = Map("test_map_1");
	
	objectsInScene = testMap.load();

	while (!glfwWindowShouldClose(window))
	{
		selectObject(window, pickingShaderProgram, platform);

		charStartPos = objectsInScene[0];

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (!isMenuOpen)
		{
			//controller.proccessInput(window, camera, objectPos, deltaTime, rotationY, rotationX);
		}
		
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();

		// populate model view projection	
		if (isMenuOpen)
		{
			view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
		}
		else
		{
			//view = glm::lookAt(camera.getCameraPos(), objectPos, camera.getCameraUp());
			view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
		}
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), window_width / window_height, 0.1f, 10000.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// populate selected
		//glUniform1i(glGetUniformLocation(shaderProgram.ID, "selected"), selected);
		shaderProgram.setVec3("lightColor", lightColor);
		shaderProgram.setVec3("lightPos", glm::vec3(5.0f, 0.0f, 0.0f));
		shaderProgram.setVec3("viewPos", camera.getCameraPos());
		//

		for (int i = 0; i < objectsInScene.size(); i++)
		{
			
			if (selectedObjectIndex == i)
			{
				glUniform1i(glGetUniformLocation(shaderProgram.ID, "selected"), selected);
			}
			else {
				glUniform1i(glGetUniformLocation(shaderProgram.ID, "selected"), false);
			}
			glm::mat4 platModel = glm::mat4(1.0f);
			platModel = glm::scale(platModel, glm::vec3(10.0f));
			platModel = glm::translate(platModel, objectsInScene.at(i));
			shaderProgram.setMat4("model", platModel);
			platform.draw(shaderProgram);
			drawFacelessCube(shaderProgram, platform.getMinAABB(), platform.getMaxAABB(), platModel);

			//will need to fix because player is also selected
			if (totalMoves >= objectsInScene.size()) totalMoves -= objectsInScene.size();
			if (i == totalMoves)
			{
				characterMove(backpack, shaderProgram);
			}
		}

		glUniform1i(glGetUniformLocation(shaderProgram.ID, "selected"), false);
		
		//dice mechanic
		diceRoll(shaderProgram, die);

		//move if dice rolled
		//characterMove()

		//Floor
		glm::mat4 planeModel = glm::mat4(1.0f);
		planeModel = glm::translate(planeModel, glm::vec3(0.0f, -5.0f, 0.0f));
		planeModel = glm::scale(planeModel, glm::vec3(1000.0f, 1000.0f, 1000.0f));
		shaderProgram.setMat4("model", planeModel);
		floor.draw(shaderProgram);
		//
		
		// Light
		lightShaderProgram.use();
		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 light = glm::mat4(1.0f);
		light = glm::translate(light, glm::vec3(5.0f, 100.0f, 0.0f));
		light = glm::scale(light, glm::vec3(1.0f, 1.0f, 1.0f));
		lightShaderProgram.setMat4("model", light);
		lightShaderProgram.setVec3("lightColor", lightColor);
		cube.draw(lightShaderProgram);
		//

		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			toggleMenu(window);
		}
		
		if (isMenuOpen)
		{
			//dummy object location
			//glm::vec3 stupid = glm::vec3(0.0f);
			debugMenu.create_menu(camera, objectsInScene[selectedObjectIndex], deltaTime, rotationY);

			camera.setCameraFront(glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch)))));

			float cameraSpeed = static_cast<float>(150 * deltaTime);

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				camera.setCameraPos(camera.getCameraPos() + cameraSpeed * camera.getCameraFront());
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				camera.setCameraPos(camera.getCameraPos() - glm::normalize(glm::cross(camera.getCameraFront(), camera.getCameraUp())) * cameraSpeed);
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				camera.setCameraPos(camera.getCameraPos() - cameraSpeed * camera.getCameraFront());
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				camera.setCameraPos(camera.getCameraPos() + glm::normalize(glm::cross(camera.getCameraFront(), camera.getCameraUp())) * cameraSpeed);
			}

		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	testMap.save(objectsInScene);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void get_resolution() 
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	/*window_width = mode->width;
	window_height = mode->height;*/

	window_width = 1024;
	window_height = 768;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	xpos = static_cast<float>(xposIn);
	ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	glm::vec4 mouse_dir = glm::vec4((2.0f * xpos) / window_width - 1.0f, 1.0f - (2.0f * ypos) / window_height, -1.0f, 1.0f);
	//glm::vec3 d = glm::unProject(screen_coords_normalized);
	glm::mat4 inverseProjMatrix = glm::inverse(projection);
	glm::vec4 ray_eye = mouse_dir * inverseProjMatrix;
	ray_eye.z = -1.0f;
	ray_eye.w = 0.0f;

	// ray casting alg
	// [0,0] center screen.
	/*glm::vec3 ray_nds = glm::vec3(x, y, z);
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	glm::vec4 test = glm::inverse(view) * ray_eye;
	ray_wor = glm::vec3(test.x,test.y,test.z);*/

	//std::cout << "screem cooords:" << x << ", " << y << ", " << z << ", " << std::endl;
	// don't forget to normalize the vector at some point
	//ray_wor = glm::normalize(ray_wor);
	//std::cout << "x = " << ray_nds.x << " y = " << ray_nds.y << " z = " << ray_nds.z << std::endl;


	

	/*glm::vec4 lRayStart_camera = ray_eye * ray_clip;									lRayStart_camera /= lRayStart_camera.w;
	glm::vec4 lRayStart_world = test * lRayStart_camera;								lRayStart_world /= lRayStart_world.w;
	glm::vec4 lRayEnd_camera = ray_eye * glm::vec4(ray_nds.x, ray_nds.y, 0.0f, 1.0f);   lRayEnd_camera /= lRayEnd_camera.w;
	glm::vec4 lRayEnd_world = test * lRayEnd_camera;									lRayEnd_world /= lRayEnd_world.w;


	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);
	out_direction = glm::normalize(lRayDir_world);

	int w_width = window_width;
	int w_height = window_height;*/



	// Reading the depth buffer
	//GLbyte color[4];
	//GLfloat depth;

	//read the color of the pixel
	//glReadPixels(xpos, 768 - ypos - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	//read the depth of the pixel
	//glReadPixels(xpos, window_height - ypos - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	//std::printf("Clicked on pixel %f, %f, color %02hhx%02hhx%02hhx%02hhx\n", xpos, ypos, color[0], color[1], color[2], color[3]);

	//glm::vec4 viewport = glm::vec4(0, 0, window_width, window_height);
	//glm::vec3 wincoord = glm::vec3(xpos, window_height - ypos - 1, depth);
	//ray_coord = glm::unProject(wincoord, view, projection, viewport);

	//std::printf("Coordinates in object space: %f, %f, %f\n", ray_coord.x, ray_coord.y, ray_coord.z);
}

void toggleMenu(GLFWwindow* window)
{
	glfwSetCursorPos(window, window_width / 2.0f, window_height / 2.0f);
	isMenuOpen = !isMenuOpen;
	if (isMenuOpen)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void drawFacelessCube(Shader& shader, glm::vec3 minAABB, glm::vec3 maxAABB, glm::mat4 model)
{
	// Define the vertices and indices for the faceless cube (edges only)
	const GLfloat vertices[] = {
		minAABB.x, minAABB.y, minAABB.z, // 0
		minAABB.x, minAABB.y, maxAABB.z, // 1
		minAABB.x, maxAABB.y, minAABB.z, // 2
		minAABB.x, maxAABB.y, maxAABB.z, // 3
		maxAABB.x, minAABB.y, minAABB.z, // 4
		maxAABB.x, minAABB.y, maxAABB.z, // 5
		maxAABB.x, maxAABB.y, minAABB.z, // 6
		maxAABB.x, maxAABB.y,maxAABB.z  // 7
	};

	// Define the indices for drawing the edges
	const GLuint indices[] = {
		// Front face
		0, 1, 1, 3, 3, 2, 2, 0,

		// Back face
		4, 5, 5, 7, 7, 6, 6, 4,

		// Sides
		0, 4, 1, 5, 2, 6, 3, 7
	};

	// Create and bind VAO, VBO, and EBO
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

	glBindVertexArray(0); // Unbind VAO

	// Use the shader program
	shader.use();
	
	shader.setMat4("model", model);

	// Bind VAO and draw the cube
	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0); // Draw 24 lines (edges of the cube)
	glBindVertexArray(0);

	// Cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		std::cout << "pressed space" << std::endl;
		isDiceRolled = true;
	}
	if (selected && key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		duplicateModel();
	}
	if (selected && key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		removeModel();
	}
}

void duplicateModel()
{
	objectsInScene.push_back(objectsInScene[selectedObjectIndex]);
}

void removeModel()
{
	std::cout << selectedObjectIndex << std::endl;

	objectsInScene.erase(objectsInScene.begin() + selectedObjectIndex);
	selectedObjectIndex = 0;
	selected = false;
}

void selectObject(GLFWwindow* window, Shader shader, Model model)
{
	if (selected)
	{
		if (glfwGetKey(window, GLFW_KEY_UP))
		{
			objectsInScene[selectedObjectIndex].y += .01f;
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN))
		{
			objectsInScene[selectedObjectIndex].y -= .01f;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT))
		{
			objectsInScene[selectedObjectIndex].x += .01f;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT))
		{
			objectsInScene[selectedObjectIndex].x -= .01f;
		}
		if (glfwGetKey(window, GLFW_KEY_J))
		{
			objectsInScene[selectedObjectIndex].z += .01f;
		}
		if (glfwGetKey(window, GLFW_KEY_K))
		{
			objectsInScene[selectedObjectIndex].z -= .01f;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			selected = false;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		//essentially the distance function squares the difference of each vertice(xyz) adds them up and returns the sqrroot.
		//sqrt((objectcoord.x - objectpos.x) * *2 + (objectcoord.y - objectpos.y) * *2 + (objectcoord.z - objectpos.z)**2);
		//std::cout << "object cooords:" << min.x << ", " << min.y << ", " << min.z << ", " << std::endl;

		//color picking lol. Slow but not noticeable especially since it's used for map building.
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		glEnableVertexAttribArray(0);

		for (int i = 0; i < objectsInScene.size(); i++)
		{
			glm::mat4 platModel = glm::mat4(1.0f);
			platModel = glm::scale(platModel, glm::vec3(10.0f));
			platModel = glm::translate(platModel, objectsInScene.at(i));
			shader.setMat4("model", platModel);

			glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			// Convert "i", the integer mesh ID, into an RGB color
			int r = (i & 0x000000FF) >> 0;
			int g = (i & 0x0000FF00) >> 8;
			int b = (i & 0x00FF0000) >> 16;

			shader.setVec4("PickingColor", glm::vec4((float)r / 255.0, (float)g / 255.0, (float)b / 255.0, 1.0f));

			model.draw(shader);
		}

		glDisableVertexAttribArray(0);
		glFlush();
		glFinish();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned char data[4];
		glReadPixels(xpos, 768 - ypos - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

		int pickedID = data[0] + (data[1] * 256) + (data[2] * 256 * 256);

		if (!(pickedID == 0x00ffffff)) 
		{
			selected = true;
			selectedObjectIndex = pickedID;

			std::ostringstream oss;
			oss << "mesh " << pickedID;
			std::cout << oss.str() << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void diceRoll(Shader shader, Model die)
{
	// Die
	glm::mat4 diceModel = glm::mat4(1.0f);
	diceModel = glm::translate(diceModel, glm::vec3(0.0f, 0.0f, 0.0f));
	if (isDiceRolled && preDiceRoll)
	{
		roll = roll_dice();
		//preDiceRoll = false;
		isDiceRolled = false;
		startTime = (float)glfwGetTime();

		startPos = totalMoves;
		totalMoves += roll + 1;
		//fix view
		diceModel = glm::rotate(diceModel, roll, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if (preDiceRoll) {
		diceModel = glm::rotate(diceModel, (float)glfwGetTime() * 10, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	drawFacelessCube(shader, die.getMinAABB(), die.getMaxAABB(), diceModel);
	shader.setMat4("model", diceModel);
	die.draw(shader);
	//
}

float roll_dice()
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

void characterMove(Model character, Shader shader)
{
	float delta = 5.0f;
	if ((float)glfwGetTime() < startTime + delta)
	{
	//A + (B -A)(t - t0)/deltaT
	//while (objectPos != objectsInScene[totalMoves])
	//while((float)glfwGetTime() - start < 10.0f)
	//for(int i = 0; i < totalMoves - startPos; i++)
	//while((float)glfwGetTime() < start + delta)
	//{
	//float playerSpeed = static_cast<float>(150 * deltaTime);
	//camera.setCameraPos(camera.getCameraPos() + playerSpeed * camera.getCameraFront());
	//test = glm::scale(test, glm::vec3(1.0f));
	
	glm::vec3 objectPos = objectsInScene[startPos] * 10.0f;

	glm::vec3 direction = (objectsInScene[totalMoves] - objectsInScene[startPos]) * 10.0f;
	std::cout << startTime + delta << " test " << (float)glfwGetTime() << std::endl;
	glm::mat4 test = glm::mat4(1.0f);

	test = glm::translate(test, objectPos  +  direction  * ((float)glfwGetTime() - startTime) / delta + glm::vec3(0, 2, 0));
	shader.setMat4("model", test);
	character.draw(shader);
	//startPos = totalMoves;

		//}
	
	}
	else {
		glm::mat4 test = glm::mat4(1.0f);
		//test = glm::scale(test, glm::vec3(1.0f));
		test = glm::translate(test, objectsInScene[totalMoves] * 10.0f + glm::vec3(0, 2, 0));
		shader.setMat4("model", test);
		character.draw(shader);
	}
}