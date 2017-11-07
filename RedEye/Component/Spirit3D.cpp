#include "Component/Component.hpp"

Spirit3D::Spirit3D(char * path)
{
	this->model = new Model(path);
	materials = new std::vector<Material*>();
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
	this->materials->push_back(material);
}

void Spirit3D::Draw()
{
	for (std::vector<Material*>::iterator it = materials->begin(); it != materials->end(); it++) {
		(*it)->Use();
	}
	this->model->DrawAllVAO();
}