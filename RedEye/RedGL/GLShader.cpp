#include "RedGL.hpp"
#include "DEBUG/Debug.hpp"

GLShader::GLShader(GLenum type, const GLchar * src)
{
    ShaderId = glCreateShader(type);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Shader
    RedLog("\nCompiling shader : \n%s\n", src);
    glShaderSource(ShaderId, 1, &src , NULL);
    glCompileShader(ShaderId);

    // Check Vertex Shader
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(ShaderId, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        RedLog("%s\n", &ShaderErrorMessage[0]);
    }
}

GLShader::~GLShader(){
    this->DeleteShader();
}

void GLShader::DeleteShader()
{
    glDeleteShader(ShaderId);
}
