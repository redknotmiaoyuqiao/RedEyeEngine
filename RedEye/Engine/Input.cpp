#include "Engine/Engine.hpp"

Input * Input::m_pInstance = NULL;

Input * Input::getInstance()
{
    if(m_pInstance == NULL){
        m_pInstance = new Input();
    }
    return m_pInstance;
}

Input::Input()
{

}

void Input::setMousePoint(float x,float y)
{
    this->mousePoint.x = x;
    this->mousePoint.y = y;
}

glm::vec2 Input::getMousePoint()
{
    return this->mousePoint;
}
