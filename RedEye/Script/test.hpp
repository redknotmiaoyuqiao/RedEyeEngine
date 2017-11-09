#pragma once

#include "Engine/Engine.hpp"
#include "Component/Component.hpp"
#include "RedLog/Debug.hpp"
#include "RedGL/RedGL.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#ifdef __ANDROID__
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#else
#include <glad/glad.h>
#endif


float vertices_s[] = {
    1.0f,1.0f,0.0f,
    1.0f,-1.0f,0.0f,
    -1.0f,-1.0f,0.0f,
    -1.0f,1.0f,0.0f
    };
float coor_s[] = {
    1.0f,1.0f,0.0f,
    1.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f
    };
unsigned int indices_s[] = {
    0,1,2,
    0,2,3
};


class Test : public RedEyeBehaviour{
private:
    GLProgram * g;
    GLProgram * s;
    GLProgram * pbr;

    GLVAO * vao_s;

    Camera * mainCamera;

    GLRenderTarget * target;
    GLRenderTarget * target_s;
    GLRenderTarget * target_screen;

    Model * teaModel;

    Light * light0;
    Light * light1;
    Light * light2;
    Light * light3;

    GLTexture * albedoMap;
    GLTexture * metallicMap;
    GLTexture * roughnessMap;
    GLTexture * normalMap;
    GLTexture * aoMap;
public:
    void Start()
    {
        glClearColor(1.0f,1.0f,1.0f,1.0f);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        g = new GLProgram(test_VERTEX,test_FRAGMENT);
        s = new GLProgram(s_VERTEX,s_FRAGMENT);
        pbr = new GLProgram(PBR_VERTEX,PBR_FRAGMENT);

        vao_s = new GLVAO();
        vao_s->AddVBO(vertices_s,sizeof(vertices_s),0);
        vao_s->AddVBO(coor_s,sizeof(coor_s),1);
        vao_s->SetEBO(indices_s,sizeof(indices_s));

        Screen * screen = Screen::getInstance();
        mainCamera = new Camera(30.0f,screen->getWidth(),screen->getHeight(),0.1f,1000.0f);

        target = new GLRenderTarget(screen->getWidth(),screen->getHeight(),RED_TARGET_SCREEN);
        target_s = new GLRenderTarget(screen->getWidth(),screen->getHeight(),RED_TARGET_TEXTURE);
        target_screen = new GLRenderTarget(screen->getWidth(),screen->getHeight(),RED_TARGET_SCREEN);

        teaModel = new Model("/Users/redknot/Red3DEngine/3dModel/Tea/model");
        //teaModel = new Model("/Users/redknot/Red3DEngine/3dModel/nano");
        //teaModel = new Model("/Users/redknot/Red3DEngine/3dModel/Tea/tea.redmeshs");
        //teaModel = new Model("/Users/redknot/Red3DEngine/3dModel/nanosuit/nanosuit.redmeshs");


        albedoMap = new GLTexture();
        albedoMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Tea/tea_DefaultMaterial_BaseColor.png");
        metallicMap = new GLTexture();
        metallicMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Tea/tea_DefaultMaterial_Metallic.png");
        roughnessMap = new GLTexture();
        roughnessMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Tea/tea_DefaultMaterial_Roughness.png");
        normalMap = new GLTexture();
        normalMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Tea/tea_DefaultMaterial_Normal.png");
        aoMap = new GLTexture();
        aoMap->LoadImage("/Users/redknot/Red3DEngine/3dModel/Tea/tea_DefaultMaterial_Height.png");



        light0 = new Light();
        light0->setColor(300.0f,300.0f,300.0f);
        light0->setPosition(-10.0f,  10.0f, 10.0f);

        light1 = new Light();
        light1->setColor(300.0f,300.0f,300.0f);
        light1->setPosition(10.0f,  10.0f, 10.0f);

        light2 = new Light();
        light2->setColor(300.0f,300.0f,300.0f);
        light2->setPosition(-10.0f, -10.0f, 10.0);

        light3 = new Light();
        light3->setColor(300.0f,300.0f,300.0f);
        light3->setPosition(10.0f, -10.0f, 10.0f);
    }

    float w = 0.0f;
    void Update()
    {
        //this->Mouse();
        //this->CameraKey();

        Screen * screen = Screen::getInstance();
        glViewport(0,0,screen->getWidth(),screen->getHeight());

        target->setWidthAndHeight(screen->getWidth(),screen->getHeight());
        target->useFrameBuffer();

        mainCamera->setCameraWidthHeight(screen->getWidth(),screen->getHeight());

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 model;
        model = glm::scale(model,glm::vec3(1.0f));
        model = glm::rotate(model,glm::radians(w),glm::vec3(0.0f,1.0f,0.0f));
        w = w + 0.5f;
        pbr->putMatrix4fv("model",glm::value_ptr(model));
        pbr->putMatrix4fv("projection",glm::value_ptr(mainCamera->getProjection()));
        pbr->putMatrix4fv("view",glm::value_ptr(mainCamera->getView()));
        pbr->put3f("camPos",mainCamera->cameraPos[0],mainCamera->cameraPos[1],mainCamera->cameraPos[2]);


            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, this->albedoMap->TextureId);
            pbr->put1i("albedoMap",0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, this->metallicMap->TextureId);
            pbr->put1i("metallicMap",1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, this->roughnessMap->TextureId);
            pbr->put1i("roughnessMap",2);

            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, this->normalMap->TextureId);
            pbr->put1i("normalMap",3);

            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, this->aoMap->TextureId);
            pbr->put1i("aoMap",4);


        light0->UseLight(pbr,0);
        light1->UseLight(pbr,1);
        light2->UseLight(pbr,2);
        light3->UseLight(pbr,3);

        pbr->UseProgram();
        teaModel->DrawAllVAO();


        /*
        target_s->setWidthAndHeight(screen->getWidth(),screen->getHeight());
        target_s->useFrameBuffer();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        s->UseProgram();
        glBindTexture(GL_TEXTURE_2D, target->getTextureId());
        vao_s->DrawVAO();
        */
    }

    void End()
    {
        RedLog("End");
        delete g;
        delete mainCamera;

        delete target;
        delete target_screen;
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

        if(firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        GLfloat sensitivity = 0.5f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);

        this->mainCamera->setCameraFront(front.x,front.y,front.z);
    }


    void CameraKey()
    {
        Input * input = Input::getInstance();

        /*
        float cameraSpeed = 0.07f;
        if (input->keys[GLFW_KEY_W])
            mainCamera->cameraPos += cameraSpeed * mainCamera->cameraFront;
        if (input->keys[GLFW_KEY_S])
            mainCamera->cameraPos -= cameraSpeed * mainCamera->cameraFront;
        if (input->keys[GLFW_KEY_A])
            mainCamera->cameraPos -= glm::normalize(glm::cross(mainCamera->cameraFront, mainCamera->cameraUp)) * cameraSpeed;
        if (input->keys[GLFW_KEY_D])
            mainCamera->cameraPos += glm::normalize(glm::cross(mainCamera->cameraFront, mainCamera->cameraUp)) * cameraSpeed;
        */
    }

};
