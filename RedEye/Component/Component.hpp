#pragma once

#include <glm/glm.hpp>
#include "RedGL/RedGL.hpp"
#include "Engine/Engine.hpp"

#ifdef __ANDROID__
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#else
#include <glad/glad.h>
#endif

#define RED_GAMEOBJECT_CAMERA 1;

class Transform;
class Material;
class Camera;

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


class Spirit3D : public GameObject{
private:
	Model * model;
	std::vector<Material*> * materials;

public:
	Spirit3D(char * path);
    ~Spirit3D();

	void AddMaterial(Material * material);

	void Draw();
};

class Skybox {
public:
	GLuint irradianceTexture;
	GLuint prefilterTexture;
	GLuint brdfLUTTexture;
public:
	GLuint getIrradianceTexture();
	GLuint getPrefilterTexture();
	GLuint getBrdfLUTTexture();

	virtual void Draw(Camera * camera) = 0;
};

class HdriSkybox : public Skybox {
private:
	GLProgram * equirectangularToCubemap;
	GLProgram * irradianceShader;
	GLProgram * background;
	GLProgram * prefilterShader;
	GLProgram * brdfShader;



	GLTexture * hdriTexture;

	GLuint envCubemap;
public:
	HdriSkybox(char * filePath);

	void Draw(Camera * camera);


private:
	GLuint cubeVAO = 0;
	GLuint cubeVBO = 0;
	void renderCube();

	GLuint quadVAO = 0;
	GLuint quadVBO = 0;
	void renderQuad();
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

	Skybox * skybox = NULL;
public:
    Camera(float fovy,float width,float height,float _near,float _far);
    ~Camera();

	void setHdriSkybox(char * path);

    void setCamera(float fovy,float width,float height,float _near,float _far);

    void setCameraWidthHeight(float width,float height);

    void setCameraPos(float x,float y,float z);
    void setCameraFront(float x,float y,float z);
    void setCameraUp(float x,float y,float z);

	void DrawSkybox();

    glm::mat4 getProjection();
    glm::mat4 getView();
};


class Material {
public:
	GLProgram * glProgram;
	std::string meshBindName;

	void setMeshBindName(std::string meshBindName);
	virtual void Use() = 0;
};

class PBRMaterial : public Material {
private:
	GLTexture * albedoMap;
	GLTexture * metallicMap;
	GLTexture * roughnessMap;
	GLTexture * normalMap;
	GLTexture * aoMap;

	Camera * camera;

public:
	PBRMaterial();
	~PBRMaterial();

	void setAlbedoMap(char * path);
	void setMetallicMap(char * path);
	void setRoughnessMap(char * path);
	void setNormalMap(char * path);
	void setAoMap(char * path);

	void setCamera(Camera * camera);

	void Use();
};
