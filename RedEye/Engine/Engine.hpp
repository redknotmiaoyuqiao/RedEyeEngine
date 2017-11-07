#pragma once

#include "Engine/Engine.hpp"
#include "RedGL/RedGL.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class Engine{
public:
    void Start();
    void Update();
    void End();
};

class RedEyeBehaviour{
public:
    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void End() = 0;
};

class Screen{
private:
    Screen();
    static Screen * m_pInstance;
    int ScreenWidth = 0;
    int ScreenHeight = 0;
public:
    static Screen * getInstance();

    void setWidthAndHeight(int width,int height);

    int getWidth();
    int getHeight();
};

class Input
{
private:
    Input();

    static Input * m_pInstance;

    glm::vec2 mousePoint;

public:
    bool keys[1024];

public:
    static Input * getInstance();

    void setMousePoint(float x,float y);
    glm::vec2 getMousePoint();
};




typedef struct Vertex
{
    float Position_x;
    float Position_y;
    float Position_z;

    float Normal_x;
    float Normal_y;
    float Normal_z;

    float Tangents_x;
    float Tangents_y;
    float Tangents_z;

    float TexCoords_x;
    float TexCoords_y;
}Vertex;

class Mesh
{
private:
    std::vector<Vertex*> * vertices;
    std::vector<GLuint> * indices;

    float * arrayVertices;
    float * arrayNormals;
    float * arrayTxtcoor;
    float * tangentsArray;

    unsigned int * arrayIndices;
public:
	char * name;

    GLVAO * vao;
    Mesh(std::vector<Vertex*> * vertices, std::vector<GLuint> * indices,char * name);
    ~Mesh();
    void DrawVAO();
};

class Model
{
public:
    std::vector<Mesh*> * meshs;
public:
    Model(std::string path);
    ~Model();
    void DrawAllVAO();
};
