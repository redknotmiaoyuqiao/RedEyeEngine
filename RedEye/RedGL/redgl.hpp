#pragma once

#include <glad/glad.h>
#include <vector>

class GLShader
{
private:
public:
    GLuint ShaderId;

    GLShader(GLenum type, const GLchar* src);
    ~GLShader();
    void DeleteShader();
};

class GLProgram
{
private:
public:
    GLuint ProgramId;
    std::vector<GLShader *> ShaderList;

    GLProgram();
    ~GLProgram();
    GLProgram(const char * vertexShaderSrc,const char * fragmentShaderSrc);

    void AddShader(GLShader * shader);
    void LinkProgram();
    void UseProgram();

    GLuint GetUniformLocation(char * name);

    void putMatrix4fv(char * name,const GLfloat* value);
    void put3f(char * name,GLfloat x,GLfloat y,GLfloat z);
    void put1i(char * name,GLint v);
    void put1f(char * name,GLfloat value);
};


class GLVAO
{
private:
public:
    GLuint VAOId;

    GLuint EBOId;

    std::vector<GLuint> VBOList;

    int DrawTime;

    GLVAO();
    ~GLVAO();
    void SetEBO(GLuint * EBOdata,int bufferSize);
    void AddVBO(GLfloat * VBOdata, int bufferSize, int layout, int size = 3, GLsizei stride = 0);
    void DrawVAO();
    void DeleteVAO();
};

class GLTexture
{
private:
public:
    GLuint TextureId;

    GLTexture();
    ~GLTexture();
    void LoadImage(char * filePath);
	void LoadHdrImage(char * filePath);
    void SetData(unsigned char * data,int width,int height,GLenum internalFormat,GLenum format);
    void SetCharData(unsigned char * data,int width,int height);
};


#define RED_TARGET_TEXTURE 0
#define RED_TARGET_SCREEN 1

class GLRenderTarget{
private:
    GLuint framebuffer;
    GLuint texColorBuffer;
    GLuint rbo;

    int target;
public:
    GLRenderTarget(int width,int height,int target);
    ~GLRenderTarget();

    void setWidthAndHeight(int width,int height);

    void useFrameBuffer();

    GLuint getTextureId();
};
