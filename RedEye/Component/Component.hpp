#pragma once

#include <glm/glm.hpp>
#include "RedGL/RedGL.hpp"

#define RED_GAMEOBJECT_CAMERA 1;

class Transform;

class GameObject{
private:
    Transform * transform;
public:
    GameObject();
    ~GameObject();
    Transform * getTransform();

    /*
    virtual int getType() = 0;
    virtual void render() = 0;
    */
};


class Transform{
private:
    glm::mat4 model;
};


class Spirit : public GameObject{
private:
public:
    Spirit();
    ~Spirit();
};

/*
 *
 * 灯光
 *
 */
class Light : public GameObject{
public:
    glm::vec3 position;
    glm::vec3 color;
public:
    Light();
    ~Light();

    glm::vec3 getPosition();
    glm::vec3 getColor();

    void setPosition(float x,float y,float z);
    void setColor(float r,float g,float b);

    void UseLight(GLProgram * program,int i);
};

/*
 *
 * Camera
 *
 */
class Camera : public GameObject
{
public:
    float * cameraPos;
    float * cameraFront;
    float * cameraUp;

    float width;
    float height;

    float fovy;
	float _near;
	float _far;

    glm::mat4 projection;
    glm::mat4 view;
public:
    Camera(float fovy,float width,float height,float _near,float _far);
    ~Camera();

    void setCamera(float fovy,float width,float height,float _near,float _far);

    void setCameraWidthHeight(float width,float height);

    void setCameraPos(float x,float y,float z);
    void setCameraFront(float x,float y,float z);
    void setCameraUp(float x,float y,float z);

    glm::mat4 getProjection();
    glm::mat4 getView();
};
