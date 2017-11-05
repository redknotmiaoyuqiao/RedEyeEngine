#include "Component/Component.hpp"

GLuint Skybox::getIrradianceTexture()
{
	return this->irradianceTexture;
}

GLuint Skybox::getPrefilterTexture()
{
	return this->prefilterTexture;
}

GLuint Skybox::getBrdfLUTTexture()
{
	return this->brdfLUTTexture;
}