#include "RedGL/RedGL.hpp"
#include "RedLog/Debug.hpp"
#include "SHADER/Shader.hpp"
#include "File/File.hpp"
#include "Script/Test.hpp"
#include "Script/PBR.hpp"
#include "Script/Sky.hpp"
#include "App/App.hpp"

RedEyeBehaviour * test;
RedEyeBehaviour * pbr;
RedEyeBehaviour * sky;

void App::Start()
{
    RedLog("Engine Start !!!!");

    sky = new Sky();
    sky->Start();
}

void App::Update()
{
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sky->Update();
}

void App::End()
{
    sky->End();

    delete sky;
}
