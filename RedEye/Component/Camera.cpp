#include "Component/Component.hpp"
#include "RedLog/Debug.hpp"
#include <glm/glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

Camera::Camera(float fovy,float width,float height,float _near,float _far)
{
    cameraPos = new float[3];
    cameraFront = new float[3];
    cameraUp = new float[3];

    cameraPos[0] = 0.0f;
    cameraPos[1] = 0.0f;
    cameraPos[2] = 15.0f;

    cameraFront[0] = 0.0f;
    cameraFront[1] = 0.0f;
    cameraFront[2] = -1.0f;

    cameraUp[0] = 0.0f;
    cameraUp[1] = 1.0f;
    cameraUp[2] = 0.0f;

    RedLog("Init Camera\nPosition:%f %f %f\nFront:   %f %f %f\nUp:      %f %f %f",0.0f,0.0f,30.0f,0.0f,0.0f,-1.0f,0.0f,1.0f,0.0f);


    this->width = width;
    this->height = height;
    this->fovy = fovy;
    this->_near = _near;
    this->_far = _far;
}

Camera::~Camera()
{
    delete cameraPos;
    delete cameraFront;
    delete cameraUp;
}

void Camera::setCamera(float fovy,float width,float height,float _near,float _far)
{
    this->width = width;
    this->height = height;
    this->fovy = fovy;
    this->_near = _near;
    this->_far = _far;
}

void Camera::setCameraWidthHeight(float width,float height)
{
    this->width = width;
    this->height = height;
}

void Camera::setCameraPos(float x,float y,float z)
{
    cameraPos[0] = x;
    cameraPos[1] = y;
    cameraPos[2] = z;
}

void Camera::setCameraFront(float x,float y,float z)
{
    cameraFront[0] = x;
    cameraFront[1] = y;
    cameraFront[2] = z;
}

void Camera::setCameraUp(float x,float y,float z)
{
    cameraUp[0] = x;
    cameraUp[1] = y;
    cameraUp[2] = z;
}

void Camera::setHdriSkybox(char * path)
{
	this->skybox = new HdriSkybox(path);
}

void Camera::DrawSkybox()
{
	if (this->skybox != NULL) {
		this->skybox->Draw(this);
	}
}

glm::mat4 Camera::getProjection()
{
    this->projection = glm::perspective(glm::radians(this->fovy), width / (height * 1.0f), _near, _far);
    return this->projection;
}

glm::mat4 Camera::getView()
{
    glm::vec3 v_cameraPos   = glm::vec3(cameraPos[0], cameraPos[1],  cameraPos[2]);
    glm::vec3 v_cameraFront = glm::vec3(cameraFront[0], cameraFront[1], cameraFront[2]);
    glm::vec3 v_cameraUp    = glm::vec3(cameraUp[0], cameraUp[1],  cameraUp[2]);

    this->view = glm::lookAt(v_cameraPos, v_cameraPos + v_cameraFront, v_cameraUp);
    return this->view;
}
