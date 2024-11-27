//
// Created by Roger on 11/11/2024.
//
#include "Component/Component.h"

#include <iostream>
#include <ostream>
#include <stdexcept>

#include "Component/GameObject.h"

ComponentPtr Component::SetGameObject(ComponentPtr& component, GameObject* newParent)
{
    if (component == nullptr)
    {
        std::cerr << "Component::SetGameObject() called with null pointer" << std::endl;
        return component;
    }
    if (newParent == nullptr)
    {
        std::cerr << "New parent is null" << std::endl;
        return component;
    }

    if (component->IsGameObject())
    {
        GameObjectPtr go = std::dynamic_pointer_cast<GameObject>(component);
        go->parent->RemoveChild(go.get());
        newParent->AddChild(go);
    }
    else
    {
        // TODO trobar una forma adequada de transferir components entre gameobjects

        // Només pot estar assignat a un GameObject
        if (component->gameObject != nullptr)
        {
            component->gameObject->RemoveComponent(component.get());
        }

        component->gameObject = newParent;
        newParent->AddComponent(component);
    }

    return component;
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
