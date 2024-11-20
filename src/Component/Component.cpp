//
// Created by Roger on 11/11/2024.
//
#include "Component/Component.h"

#include <stdexcept>

#include "Component/GameObject.h"

Component& Component::SetGameObject(GameObject* parent)
{
    if (parent == nullptr)
    {
        return *this;
    }

    // Només pot estar assignat a un GameObject
    if (this->gameObject != nullptr)
    {
        this->gameObject->RemoveComponent(this);
    }

    if (this->gameObject != parent)
    {
        this->gameObject = parent;
        parent->AddComponent(this);
    }

    return *this;
}


Component::Component(GameObject* parent)
{
    SetGameObject(parent);
}

bool Component::Enable()
{
    if (!_enabled)
    {
        _enabled = true;
    }
    return _enabled;
}

bool Component::Disable()
{
    if (_enabled)
    {
        _enabled = false;
    }
    return _enabled;
}
