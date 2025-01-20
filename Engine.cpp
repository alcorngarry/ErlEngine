#include"Engine.h"

float lastFrame = 0.0f;
float deltaTime = 0.0f;

Game* game; // game project
GLFWwindow* window; // backend

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void Engine::run()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	void get_resolution();

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	window = glfwCreateWindow(mode->width, mode->height, "test_game", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	Renderer::init_render(window);
	InputManager::init(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	game = new Game(window);
	game->init();
	
	//need some kind of editor class...eventually
	InputManager::set_key_binding(GLFW_KEY_F9, new SaveCommand(game->Maps[game->level]));
	InputManager::set_key_binding(GLFW_KEY_M, new ToggleMenuCommand(game));
	InputManager::set_key_binding(GLFW_KEY_ESCAPE, new CloseWindowCommand(window));

	start_game_loop();
}

void Engine::start_game_loop()
{
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		game->update(deltaTime);

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game->render(deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();

		InputManager::update();
	}
	//move eventually
	glfwDestroyWindow(window);
	glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}