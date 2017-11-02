#include "Component/Component.hpp"

GameObject::GameObject()
{
    this->transform = new Transform();
}

GameObject::~GameObject()
{
    delete this->transform;
}

Transform * GameObject::getTransform()
{
    return this->transform;
}
