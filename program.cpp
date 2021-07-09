#include <stdlib.h> // aleatorios
#include <time.h> // aleatorios
#include <typedef.h>

#include "cam.h"
#include "game.h"

// settings
const i32 SCR_WIDTH = 800;
const i32 SCR_HEIGHT = 600;
const i32 SCR_DEPTH = 600;

const f32  ASPECT = (f32)SCR_WIDTH / (f32)SCR_HEIGHT;

Game Space(500, 500, 500);
Cam* cam;

// deltatime variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// declarations
void key_callback(GLFWwindow* window, int key, int scanmode, int action, int mode) {
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Space.Keys[key] = true;
		else if (action == GLFW_RELEASE)
			Space.Keys[key] = false;
	}
}
void processInput(GLFWwindow* window) {
	f32 auxtime = deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		auxtime *= 30;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam->processKeyboard(CamMove::FORWARD, auxtime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam->processKeyboard(CamMove::BACKWARD, auxtime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam->processKeyboard(CamMove::LEFT, auxtime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam->processKeyboard(CamMove::RIGHT, auxtime);
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		cam->processKeyboard(CamMove::UP, auxtime);
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		cam->processKeyboard(CamMove::DOWN, auxtime);
	}
}
void mouse_callback(GLFWwindow* window, f64 xpos, f64 ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		cam->movePov(xpos, ypos);
	}
	else {
		cam->stopPov();
	}
}
void scroll_callback(GLFWwindow* window, f64 xoffset, f64 yoffset) {
	cam->processScroll((f32)yoffset);
}

i32 main(int argc, char *argv[]) {

	GLFWwindow* window = glutilInit(3, 3, SCR_WIDTH, SCR_HEIGHT, "Space");
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glEnable(GL_DEPTH_TEST);

	cam = new Cam();

	// opengl configuration
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initalize game
	Space.Init();

	while (!glfwWindowShouldClose(window)) {
		
		// per-frame logic
		f32 currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();
		processInput(window);

		// proyection and view
		glm::mat4 projection = glm::perspective(cam->zoom, ASPECT, 0.1f, 1000.0f);
		ResourceManager::GetShader("sprite")->setMat4("proj", projection);
		ResourceManager::GetShader("sprite")->setMat4("view", cam->getViewM4());


		// manage user input
		Space.ProcessInput(deltaTime);

		// update game state
		Space.Update(deltaTime);

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Space.Render();

		glfwSwapBuffers(window);
	}

	glfwTerminate();


	return 0;
}

