#include "Component/Component.hpp"
#include <string>
#include <iostream>
#include "RedLog/Debug.hpp"

Light::Light()
{

}

Light::~Light()
{

}

void Light::setPosition(float x,float y,float z)
{
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
}

void Light::setColor(float r,float g,float b)
{
    this->color.r = r;
    this->color.g = g;
    this->color.b = b;
}

glm::vec3 Light::getPosition()
{
    return this->position;
}

glm::vec3 Light::getColor()
{
    return this->color;
}


void Light::UseLight(GLProgram * program,int i)
{
    const char * position_str = ("lightPositions[" + intToString(i) + "]").c_str();
    const char * color_str = ("lightColors[" + intToString(i) + "]").c_str();

    //RedLog("%s\n",color_str);

    glUniform3f(program->GetUniformLocation((char *)position_str), position[0], position[1], position[2]);
    glUniform3f(program->GetUniformLocation((char *)color_str), color[0], color[1], color[2]);
}
