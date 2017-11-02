#include "RedGL/RedGL.hpp"
#include "RedLog/Debug.hpp"
#include <glad/glad.h>

GLRenderTarget::GLRenderTarget(int width,int height,int target)
{
    this->target = target;

    glViewport(0,0,width,height);

    if(this->target == RED_TARGET_TEXTURE){
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glGenTextures(1, &texColorBuffer);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            RedLog("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
        }
    }
    else if(this->target == RED_TARGET_SCREEN){
        framebuffer = 0;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLRenderTarget::~GLRenderTarget()
{

}

void GLRenderTarget::setWidthAndHeight(int width,int height)
{
    glViewport(0,0,width,height);
    if(this->target == RED_TARGET_TEXTURE){
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    }
}

void GLRenderTarget::useFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
}

GLuint GLRenderTarget::getTextureId()
{
    return this->texColorBuffer;
}
