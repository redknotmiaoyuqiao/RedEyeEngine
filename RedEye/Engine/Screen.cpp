#include "Engine/Engine.hpp"
#include "RedLog/Debug.hpp"
#include <iostream>

Screen * Screen::m_pInstance = NULL;

Screen::Screen(){
    this->ScreenWidth = 0;
    this->ScreenHeight = 0;
}

Screen * Screen::getInstance()
{
    if(m_pInstance == NULL){
        m_pInstance = new Screen();
    }
    return m_pInstance;
}

void Screen::setWidthAndHeight(int width,int height)
{
    this->ScreenWidth = width;
    this->ScreenHeight = height;

    RedLog("Screen Width: %d",this->ScreenWidth);
    RedLog("Screen Height: %d",this->ScreenHeight);
}

int Screen::getWidth()
{
    return this->ScreenWidth;
}

int Screen::getHeight()
{
    return this->ScreenHeight;
}
