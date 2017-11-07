#include "Engine.hpp"
#include "RedGL/RedGL.hpp"
#include "RedLog/Debug.hpp"
#include "SHADER/Shader.hpp"
#include "File/File.hpp"
#include "Script/Test.hpp"
#include "Script/PBR.hpp"
#include "Script/Sky.hpp"

RedEyeBehaviour * test;
RedEyeBehaviour * pbr;

RedEyeBehaviour * sky;

void Engine::Start()
{
    RedLog("Engine Start !!!!");

	sky = new Sky();
	sky->Start();

}

void Engine::Update()
{
	sky->Update();
}

void Engine::End()
{
	sky->End();

	delete sky;
}
