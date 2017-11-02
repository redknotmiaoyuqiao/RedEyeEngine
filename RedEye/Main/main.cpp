#include "DEBUG/Debug.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>  

#include "Engine/Engine.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
	RedLog("Welcame RedEye!!!!\n\n");

	RedLog("Initialize GLFW\n");
	if (!glfwInit())
	{
		RedLog("Failed to initialize GLFW\n");
		return -1;
	}
	RedLog("Initialize GLFW Success!!!\n");

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//不可改变大小
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//没有边框
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	GLFWwindow * window;
	int width = 2560;
	int height = 1600;

	bool isFullScreen = false;
	GLFWmonitor* pMonitor = isFullScreen ? glfwGetPrimaryMonitor() : NULL;

	window = glfwCreateWindow(width, height, "RedEye", pMonitor, NULL);
	if (window == NULL) {
		RedLog("Failed to open GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//设置监听
	RedLog("GLFW Init Callback");
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);

	Screen * screen = Screen::getInstance();
	screen->setWidthAndHeight(scrWidth, scrHeight);

	// glad初始化  
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("加载失败");
		return -1;
	}
	/*
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
	RedLog("Failed to initialize GLEW\n");
	glfwTerminate();
	return -1;
	}
	*/

	Engine * RedEngine = new Engine();
	RedEngine->Start();

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
	{
		RedEngine->Update();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	RedEngine->End();

	delete RedEngine;

	glfwTerminate();

	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Screen * screen = Screen::getInstance();
	screen->setWidthAndHeight(width, height);

	RedLog("Screen Size Changed!!!");
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	Input * input = Input::getInstance();
	input->setMousePoint(xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	Input * input = Input::getInstance();
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) {
			input->keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			input->keys[key] = false;
		}
	}
}

