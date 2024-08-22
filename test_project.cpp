#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"Model.h"
#include"CharacterController.h"
#include"DebugMenu.h"
#include"Map.h"

#include<GLFW/glfw3.h>

CharacterController controller;
Camera camera;

glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 objcoord;

float deltaTime = 0.0f, lastFrame = 0.0f, rotationY = 0.0f, rotationX = 0.0f;
float window_width, window_height;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void get_resolution();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void toggleMenu(GLFWwindow* window, DebugMenu debugMenu);


unsigned int texture1, texture2;
glm::mat4 projection, view;
glm::vec3 ray_wor, front;

bool selected = false;
bool firstMouse = true;
bool isMenuOpen = false;

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = window_width / 2.0;
float lastY = window_height / 2.0;
float fov = 60.0f;

unsigned int id;

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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader shaderProgram("default.vert.glsl", "default.frag.glsl");
	Shader lightShaderProgram("light.vert.glsl", "light.frag.glsl");

	id = shaderProgram.ID;

	Model backpack((char*)"C:/Users/alcor/Downloads/LearnOpenGL-master/LearnOpenGL-master/resources/objects/backpack/backpack.obj");
	Model cube((char*)"C:/Dev/assets/cube.glb");
	Model floor((char*)"C:/Dev/assets/plane/plane.obj");
	Model test((char*)"C:/Dev/assets/untitled.obj");


	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	camera = Camera();
	controller = CharacterController();
	DebugMenu debugMenu = DebugMenu(window);

	Map testMap = Map("test_map_1");

	
	objectPos = testMap.load();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (!isMenuOpen)
		{
			controller.proccessInput(window, camera, objectPos, deltaTime, rotationY, rotationX);

		}
		
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();

		if (selected)
		{

			glm::mat4 model3 = glm::mat4(1.0f);
			model3 = glm::translate(model3, objectPos);
			model3 = glm::rotate(model3, (float)glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			shaderProgram.setMat4("model", model3);
			test.draw(shaderProgram);

			if (glfwGetKey(window, GLFW_KEY_UP))
			{
				objectPos.y += .01f;
			}

			if (glfwGetKey(window, GLFW_KEY_DOWN))
			{
				objectPos.y -= .01f;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT))
			{
				objectPos.x += .01f;
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT))
			{
				objectPos.x -= .01f;
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

			if (glm::distance(objcoord, objectPos) < 4)
			{
				std::cout << "hit" << std::endl;
				selected = true;
			}
		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, objectPos);
		model = glm::rotate(model, rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		projection = glm::mat4(1.0f);
		
		if (isMenuOpen)
		{
			view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getCameraUp());
		}
		else
		{
			view = glm::lookAt(camera.getCameraPos(), objectPos, camera.getCameraUp());
		}


		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), window_width / window_height, 0.1f, 10000.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "selected"), selected);

		shaderProgram.setMat4("model", model);
		shaderProgram.setVec3("lightColor", lightColor);
		shaderProgram.setVec3("lightPos", glm::vec3(5.0f, 0.0f, 0.0f));
		shaderProgram.setVec3("viewPos", camera.getCameraPos());

		backpack.draw(shaderProgram);

		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(0.0f, -5.0f, 0.0f));
		model2 = glm::scale(model2 , glm::vec3(1000.0f, 1000.0f, 1000.0f));
		shaderProgram.setMat4("model", model2);

		glUniform1i(glGetUniformLocation(shaderProgram.ID, "selected"), false);
		floor.draw(shaderProgram);

		lightShaderProgram.use();

		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(lightShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 light = glm::mat4(1.0f);
		light = glm::translate(light, glm::vec3(5.0f, 0.0f, 0.0f));
		light = glm::scale(light, glm::vec3(1.0f, 1.0f, 1.0f));

		lightShaderProgram.setMat4("model", light);
		lightShaderProgram.setVec3("lightColor", lightColor);
		
		cube.draw(lightShaderProgram);

		light = glm::translate(light, glm::vec3(10.0f, 10.0f, -10.0f));
		light = glm::scale(light, glm::vec3(1.0f, 1.0f, 1.0f));

		lightShaderProgram.setMat4("model", light);
		lightShaderProgram.setVec3("lightColor", glm::vec3(0.5f, 0.0f, 0.0f));

		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			toggleMenu(window, debugMenu);
		}
		
		if (isMenuOpen)
		{
			debugMenu.create_menu(camera, objectPos, deltaTime, rotationY);

			camera.setCameraFront(glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch)))));

			float cameraSpeed = static_cast<float>(10 * deltaTime);

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
		
		cube.draw(lightShaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	testMap.save(objectPos);

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

	window_width = mode->width;
	window_height = mode->height;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

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
	front = glm::normalize(front); //deriv

	float x = (2.0f * xpos) / window_width - 1.0f;
	float y = 1.0f - (2.0f * ypos) / window_height;
	float z = 1.0f;
	
	// ray casting alg
	// [0,0] center screen.
	glm::vec3 ray_nds = glm::vec3(x, y, z);
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	glm::vec4 test = glm::inverse(view) * ray_eye;
	ray_wor = glm::vec3(test.x,test.y,test.z);
	// don't forget to normalize the vector at some point
	ray_wor = glm::normalize(ray_wor);
	//std::cout << "x = " << ray_nds.x << " y = " << ray_nds.y << " z = " << ray_nds.z << std::endl;

	int w_width = window_width;
	int w_height = window_height;

	GLbyte color[4];
	GLfloat depth;

	//read the color of the pixel
	glReadPixels(xpos, w_height - ypos - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	//read the depth of the pixel
	glReadPixels(xpos, w_height - ypos - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	std::printf("Clicked on pixel %f, %f, color %02hhx%02hhx%02hhx%02hhx, depth %f\n", xpos, ypos, color[0], color[1], color[2], color[3], depth);

	glm::vec4 viewport = glm::vec4(0, 0, w_width, w_height);
	glm::vec3 wincoord = glm::vec3(xpos, w_height - ypos - 1, depth);
	objcoord = glm::unProject(wincoord, view, projection, viewport);

	std::printf("Coordinates in object space: %f, %f, %f\n", objcoord.x, objcoord.y, objcoord.z);

	if (selected)
	{	
		objectPos = objcoord;
	}
}

void toggleMenu(GLFWwindow* window, DebugMenu debugMenu)
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