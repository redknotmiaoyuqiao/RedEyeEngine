TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

unix:macx
{
    LIBS+= -framework opengl
    LIBS+= -framework Cocoa -framework IOKit -framework CoreVideo

    LIBS+= -L/usr/local/lib -lglfw3
}

INCLUDEPATH += /usr/local/include
INCLUDEPATH += glad/include
INCLUDEPATH += glm/

HEADERS += \
    Component/Component.hpp \
    Engine/Engine.hpp \
    File/File.hpp \
    File/IMAGE/stb_image.h \
    RedGL/RedGL.hpp \
    RedLog/Debug.hpp \
    Script/PBR.hpp \
    Script/test.hpp \
    SHADER/Header.hpp \
    SHADER/Shader.hpp \
    Script/Sky.hpp \
    App/App.hpp

SOURCES += \
    Component/Camera.cpp \
    Component/GameObject.cpp \
    Component/HdriSkybox.cpp \
    Component/Light.cpp \
    Component/Skybox.cpp \
    Engine/Input.cpp \
    Engine/Mesh.cpp \
    Engine/Model.cpp \
    Engine/Screen.cpp \
    File/IMAGE/stb_image.cpp \
    File/ImageFile.cpp \
    Main/main.cpp \
    RedGL/GLProgram.cpp \
    RedGL/GLRenderTarget.cpp \
    RedGL/GLShader.cpp \
    RedGL/GLTexture.cpp \
    RedGL/GLVAO.cpp \
    glad/src/glad.c \
    Component/Material.cpp \
    Component/PBRMaterial.cpp \
    Component/Spirit3D.cpp \
    App/App.cpp
