#include "Engine.hpp"
#include "RedGL/RedGL.hpp"
#include "RedLog/Debug.hpp"
#include "SHADER/Shader.hpp"
#include "File/File.hpp"
#include "Script/Test.hpp"
#include "Script/PBR.hpp"

RedEyeBehaviour * test;
RedEyeBehaviour * pbr;

void Engine::Start()
{
    RedLog("Engine Start !!!!");

    //test = new Test();
    pbr = new PBR();
    //test->Start();
    pbr->Start();
}

void Engine::Update()
{
    //test->Update();
    pbr->Update();
}

void Engine::End()
{
    //test->End();
    //delete test;

    pbr->End();

    delete pbr;
}
