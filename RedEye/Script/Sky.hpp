#pragma once

#include "File/File.hpp"
#include "Engine/Engine.hpp"
#include "Component/Component.hpp"
#include "RedLog/Debug.hpp"
#include "RedGL/RedGL.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <File/IMAGE/stb_image.h>


#ifdef __ANDROID__
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#else
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#endif


class Sky : public RedEyeBehaviour {
public:
	GLRenderTarget * target;
	Screen * screen = Screen::getInstance();

	Camera * mainCamera;



	Spirit3D * ball;
	PBRMaterial * mm;
	PBRMaterial * metal;
	PBRMaterial * metal2;

	PBRMaterial * wood;
	void Start() {
		glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		target = new GLRenderTarget(screen->getWidth(), screen->getHeight(), RED_TARGET_SCREEN);
		mainCamera = new Camera(30.0f, screen->getWidth(), screen->getHeight(), 0.1f, 1000.0f);

        //mainCamera->setHdriSkybox("/Users/redknot/RedEyeEngine/res/hdri/hdri1.hdr");
        //mainCamera->setHdriSkybox(File::getResPathCStr("hdri/hdri1.hdr"));
		//mainCamera->setHdriSkybox("c://RedEyeEngine/res/hdri/hdri1.hdr");
        mainCamera->setHdriSkybox(File::getResPathCStr("hdri/newport_loft.hdr"));

		
		/*
		mm = new PBRMaterial();
        mm->meshBindName = "part0";
		mm->setCamera(mainCamera);
        mm->setAlbedoMap(File::getResPathCStr("3dcoat/T2/export3dcoat_lambert3SG_BaseColor.jpg"));
        mm->setMetallicMap(File::getResPathCStr("3dcoat/T2/export3dcoat_lambert3SG_BaseColor.jpg"));
        mm->setRoughnessMap(File::getResPathCStr("3dcoat/T2/export3dcoat_lambert3SG_BaseColor.jpg"));
        mm->setNormalMap(File::getResPathCStr("3dcoat/T2/export3dcoat_lambert3SG_Normal.jpg"));
        mm->setAoMap(File::getResPathCStr("3dcoat/T2/export3dcoat_lambert3SG_Metallic.jpg"));
		*/
		



		wood = new PBRMaterial();
		wood->meshBindName = "part2";
		wood->setCamera(mainCamera);
        wood->setAlbedoMap(File::getResPathCStr("flintlock/wood_albedo.jpg"));
        wood->setMetallicMap(File::getResPathCStr("flintlock/wood_metallic.jpg"));
        wood->setRoughnessMap(File::getResPathCStr("flintlock/wood_roughness.jpg"));
        wood->setNormalMap(File::getResPathCStr("flintlock/wood_normal.jpg"));
        wood->setAoMap(File::getResPathCStr("flintlock/wood_AO.jpg"));

		metal = new PBRMaterial();
		metal->meshBindName = "part0";
		metal->setCamera(mainCamera);
        metal->setAlbedoMap(File::getResPathCStr("flintlock/metalmain_albedo.jpg"));
        metal->setMetallicMap(File::getResPathCStr("flintlock/metalmain_metallic.jpg"));
        metal->setRoughnessMap(File::getResPathCStr("flintlock/metalmain_roughness.jpg"));
        metal->setNormalMap(File::getResPathCStr("flintlock/metalmain_normal.jpg"));
        metal->setAoMap(File::getResPathCStr("flintlock/metalmain_AO.jpg"));

		metal2 = new PBRMaterial();
		metal2->meshBindName = "part1";
		metal2->setCamera(mainCamera);
        metal2->setAlbedoMap(File::getResPathCStr("flintlock/Metal2_albedo.jpg"));
        metal2->setMetallicMap(File::getResPathCStr("flintlock/Metal2_metallic.jpg"));
        metal2->setRoughnessMap(File::getResPathCStr("flintlock/Metal2_roughness.jpg"));
        metal2->setNormalMap(File::getResPathCStr("flintlock/Metal2_normal.jpg"));
        metal2->setAoMap(File::getResPathCStr("flintlock/Metal2_AO.jpg"));

        ball = new Spirit3D(File::getResPathCStr("flintlock/M"));
		//ball = new Spirit3D(File::getResPathCStr("3dcoat/M"));
        //ball->AddMaterial(mm);

		ball->AddMaterial(wood);
		ball->AddMaterial(metal);
		ball->AddMaterial(metal2);

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
		//yaw += 0.1f;
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
