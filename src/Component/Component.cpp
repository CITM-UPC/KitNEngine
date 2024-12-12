//
// Created by Roger on 11/11/2024.
//
#include "Component/Component.h"

#include <iostream>
#include <ostream>
#include <stdexcept>

#include "Component/GameObject.h"

Component::Component(const std::string& name) : _name(name)
{
}

bool Component::Start()
{
    return _active;
}

bool Component::PreUpdate()
{
    _active = _enabled;
    return _active;
}

bool Component::Update()
{
    return _active;
}

bool Component::PostUpdate()
{
    return _active;
}

bool Component::InspectorDisplay()
{
    return true;
}

bool Component::CleanUp()
{
    return true;
}

// NOTE Aixo probablement necessiti un refactor
std::shared_ptr<Component> Component::SetGameObject(std::shared_ptr<GameObject>& newParent,
                                                    std::shared_ptr<Component>& component)
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
        std::shared_ptr<GameObject> go = std::dynamic_pointer_cast<GameObject>(component);
        if (go->parent != nullptr)
            go->parent->RemoveChild(go);
        newParent->AddChild(go);
    }
    else
    {
        // TODO trobar una forma mes adequada de transferir components entre gameobjects

        // Només pot estar assignat a un GameObject
        std::shared_ptr<GameObject> g = component->gameObject.lock();
        if (g != nullptr)
        {
            g->RemoveComponent(component);
        }

        component->gameObject = newParent;
        newParent->AddComponent(component);
    }

    return component;
}

bool Component::IsGameObject() const
{
    return false;
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

bool Component::IsActive() const
{
    return _active;
}

std::string Component::GetName() const
{
    return _name;
}

std::shared_ptr<GameObject> Component::GetGameObject(bool exceptOnNull) const
{
    std::shared_ptr<GameObject> g = gameObject.lock();
    if (g == nullptr && exceptOnNull)
    {
        throw std::runtime_error("Component::GetGameObject(): Component not attached to a GameObject");
    }
    return g;
}

std::shared_ptr<Component> Component::GetSmartPtr() const
{
    return _selfPtr.lock();
}
