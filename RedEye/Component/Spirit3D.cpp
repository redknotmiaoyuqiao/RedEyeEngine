#include "Component/Component.hpp"
#include "RedLog/Debug.hpp"

Spirit3D::Spirit3D(char * path)
{
	this->model = new Model(path);
	materials = new std::vector<Material*>();

	for (std::vector<Mesh*>::iterator it = model->meshs->begin(); it != model->meshs->end(); it++) {
		materials->push_back(NULL);
	}
}

Spirit3D::~Spirit3D()
{
	for (std::vector<Material*>::iterator it = materials->begin(); it != materials->end(); it++) {
		delete *it;
	}
	materials->clear();
	delete materials;

	delete model;
}

void Spirit3D::AddMaterial(Material * material)
{
	//this->materials->push_back(material);
	int size = model->meshs->size();
	for (int i = 0; i < size;i++) {
		bool isSame = (material->meshBindName == model->meshs->at(i)->name);
		if (isSame) {
			materials->at(i) = material;
			RedLog("\n---------------------------\nMesh Name:%s\nMaterial Name:%s\nisSame:%d\n---------------------------\n", model->meshs->at(i)->name.c_str(), materials->at(i)->meshBindName.c_str(), isSame);
		}
	}
}

void Spirit3D::Draw()
{
	int size = model->meshs->size();
	for (int i = 0; i < size; i++) {
		if (materials->at(i) != NULL) {
			materials->at(i)->Use();
		}
		model->meshs->at(i)->DrawVAO();
	}
}