#include "Component/Component.hpp"
#include "Shader/Shader.hpp"
#include <glm/glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

PBRMaterial::PBRMaterial()
{
	glProgram = new GLProgram(PBR_VERTEX, PBR_FRAGMENT);

	this->albedoMap = new GLTexture();
	this->metallicMap = new GLTexture();
	this->roughnessMap = new GLTexture();
	this->normalMap = new GLTexture();
	this->aoMap = new GLTexture();
}

PBRMaterial::~PBRMaterial()
{
	delete glProgram;

	delete albedoMap;
	delete metallicMap;
	delete roughnessMap;
	delete normalMap;
	delete aoMap;
}

void PBRMaterial::setAlbedoMap(char * path)
{
	albedoMap->LoadImage(path);
}

void PBRMaterial::setMetallicMap(char * path)
{
	metallicMap->LoadImage(path);
}

void PBRMaterial::setRoughnessMap(char * path)
{
	roughnessMap->LoadImage(path);
}

void PBRMaterial::setNormalMap(char * path)
{
	normalMap->LoadImage(path);
}

void PBRMaterial::setAoMap(char * path)
{
	aoMap->LoadImage(path);
}

void PBRMaterial::setCamera(Camera * camera)
{
	this->camera = camera;
}

void PBRMaterial::Use()
{
	glProgram->UseProgram();

	glm::mat4 model;
	model = glm::scale(model, glm::vec3(0.3f));
	//model = glm::rotate(model,glm::radians(90.0f),glm::vec3(-1.0f,0.0f,0.0f));
	glProgram->putMatrix4fv("model", glm::value_ptr(model));
	glProgram->putMatrix4fv("projection", glm::value_ptr(camera->getProjection()));
	glProgram->putMatrix4fv("view", glm::value_ptr(camera->getView()));
	glProgram->put3f("camPos", camera->cameraPos[0], camera->cameraPos[1], camera->cameraPos[2]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->albedoMap->TextureId);
	glProgram->put1i("albedoMap", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->metallicMap->TextureId);
	glProgram->put1i("metallicMap", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->roughnessMap->TextureId);
	glProgram->put1i("roughnessMap", 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, this->normalMap->TextureId);
	glProgram->put1i("normalMap", 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, this->aoMap->TextureId);
	glProgram->put1i("aoMap", 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->camera->skybox->getIrradianceTexture());
	glProgram->put1i("irradianceMap", 5);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->camera->skybox->getPrefilterTexture());
	glProgram->put1i("prefilterMap", 6);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, this->camera->skybox->getBrdfLUTTexture());
	glProgram->put1i("brdfLUT", 7);
}