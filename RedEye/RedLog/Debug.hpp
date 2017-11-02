#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>

#ifdef __ANDROID__
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h> 
#endif

#ifdef __ANDROID__

#include <android/log.h>
#define LOG_TAG "opengl"
#define RedLog(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#else

#define RedLog(format, ...) RedPrintf(__FILE__, __FUNCTION__ ,__LINE__,format, ##__VA_ARGS__)
#define BaseRedLog(format, ...) printf(format, ##__VA_ARGS__)

#endif

#define glCheckError() glCheckError_(__FILE__, __LINE__)

using namespace std;


static void RedPrintf(const char *file, const char *function,int line,char * format,...)
{
    BaseRedLog("\033[;31m%s (%d) \033[0m",file,line);

    va_list ap;
    char *p;
    char *sval;
    int ival;
    double dval;
    va_start(ap,format);

    for(p=format;*p;p++)
    {
        if(*p!='%')
        {
            putchar(*p);
            continue;
        }
        switch(*++p)
        {
        case 'd':
            ival=va_arg(ap,int);
            BaseRedLog("%d",ival);
            break;
        case 'f':
            dval=va_arg(ap,double);
            BaseRedLog("%f",dval);
            break;
        case 's':
            sval=va_arg(ap,char *);
            BaseRedLog("%s",sval);
            break;
        default:
            putchar(*p);
            break;
        }
    }
    va_end(ap);  //结束时清理工作
    BaseRedLog("\n");


}

static string intToString(int v)
{
    char buf[32] = {0};
    snprintf(buf, sizeof(buf), "%u", v);

    string str = buf;
    return str;
}

static GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        char * error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        //std::cout << error << " | " << file << " (" << line << ")" << std::endl;

        RedLog("%s | %s (%d)\n",error,file,line);
    }
    return errorCode;
}

