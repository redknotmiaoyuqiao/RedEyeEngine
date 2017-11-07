#pragma once

#include "Engine/Engine.hpp"
#include "Component/Component.hpp"
#include "RedLog/Debug.hpp"
#include "RedGL/RedGL.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <File/IMAGE/stb_image.h>

class Sky : public RedEyeBehaviour {
public:
	GLRenderTarget * target;
	Screen * screen = Screen::getInstance();

	Camera * mainCamera;



	Spirit3D * ball;
	PBRMaterial * mm;


	GLTexture * albedoMap;

	void Start() {
		//glEnable(GL_CULL_FACE);
		//glFrontFace(GL_CCW);

		target = new GLRenderTarget(screen->getWidth(), screen->getHeight(), RED_TARGET_SCREEN);
		mainCamera = new Camera(30.0f, screen->getWidth(), screen->getHeight(), 0.1f, 1000.0f);

		mainCamera->setHdriSkybox("c://Red3DEngine/Texture/hdr/newport_loft.hdr");

		mm = new PBRMaterial();
		mm->setCamera(mainCamera);
		mm->setAlbedoMap("C://RedEyeEngine/res/3dcoat/color.jpg");
		mm->setMetallicMap("C://RedEyeEngine/res/3dcoat/metalness.jpg");
		mm->setRoughnessMap("C://RedEyeEngine/res/3dcoat/gloss.jpg");
		mm->setNormalMap("C://RedEyeEngine/res/3dcoat/nmap.jpg");
		mm->setAoMap("C://RedEyeEngine/res/3dcoat/ao.jpg");

		//ball = new Spirit3D("C://RedEyeEngine/res/3dcoat/M");
		ball = new Spirit3D("D://stg/M");
		ball->AddMaterial(mm);
	}
	void Update() {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		Mouse();
		WASD();

		target->setWidthAndHeight(screen->getWidth(), screen->getHeight());
		target->useFrameBuffer();
		mainCamera->setCameraWidthHeight(screen->getWidth(), screen->getHeight());

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mainCamera->DrawSkybox();

		ball->Draw();
	}
	void End() {
		delete target;
		delete mainCamera;
	}

private:
	bool firstMouse = true;
	float lastX = 0.0f;
	float lastY = 0.0f;
	float yaw;
	float pitch;
	void Mouse()
	{
		Input * input = Input::getInstance();

		float xpos = input->getMousePoint().x;
		float ypos = input->getMousePoint().y;

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
			return;
		}

		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		GLfloat sensitivity = 0.2f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front);

		this->mainCamera->setCameraFront(front.x, front.y, front.z);
	}


	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -15.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	void WASD() {
		Input * input = Input::getInstance();
		GLfloat cameraSpeed = 0.1f;
		if (input->keys[GLFW_KEY_W]) {
			cameraPos += cameraSpeed * cameraFront;
		}
		if (input->keys[GLFW_KEY_S]) {
			cameraPos -= cameraSpeed * cameraFront;
		}
		if (input->keys[GLFW_KEY_A]) {
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (input->keys[GLFW_KEY_D]) {
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}

		cameraFront.x = mainCamera->cameraFront[0];
		cameraFront.y = mainCamera->cameraFront[1];
		cameraFront.z = mainCamera->cameraFront[2];

		mainCamera->cameraPos[0] = cameraPos.x;
		mainCamera->cameraPos[1] = cameraPos.y;
		mainCamera->cameraPos[2] = cameraPos.z;
	}
};