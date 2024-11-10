//
// Created by Roger on 6/11/2024.
//

#include "Component/GameObject.h"

#include <imgui.h>

std::vector<GameObjectPtr> GameObject::gameObjects = std::vector<GameObjectPtr>();

GameObject::GameObject(GameObjectPtr go) : parent(go)
{
}

bool GameObject::Awake()
{
    if (!Component::Awake()) return false;
    return true;
}

bool GameObject::Start()
{
    if (!Component::Start()) return false;
    return true;
}

bool GameObject::PreUpdate()
{
    if (!Component::PreUpdate()) return false;
    return true;
}

bool GameObject::Update()
{
    if (!Component::Update()) return false;
    return true;
}

bool GameObject::PostUpdate()
{
    if (!Component::PostUpdate()) return false;

    for (GameObjectPtr& go : GameObject::gameObjects)
    {
        if (go->_active && !go->PostUpdate())
            return false;
    }

    return true;
}

bool GameObject::InspectorDisplay()
{
    //Component::InspectorDisplay();
    ImGui::Text("GameObject");
    
    for (ComponentPtr& component : components)
    {
        if (component->InspectorDisplay()) return false;
    }
    
    return true;
}

bool GameObject::CleanUp()
{
    bool ret = true;

    for (ComponentPtr& component : components)
    {
        ret &= component->CleanUp();
    }
    
    for (GameObjectPtr& go : children)
    {
        ret &=go->CleanUp();
    }

    children.clear();
    
    ret &= Component::CleanUp();

    return ret;
}

template <class T>
std::shared_ptr<T> GameObject::GetComponentOfType() const
{
    static_assert(std::is_base_of_v<Component, T>, "T ha de derivar de Component");
    static_assert(!std::is_base_of_v<GameObject, T>, "T NO ha de ser un GameObject. Utilitza GetChild per obtenir un GameObject");

    for (const ComponentPtr& component : components) {
        // Attempt to cast to the desired type and return it if successful
        if (std::shared_ptr<T> castedComponent = std::dynamic_pointer_cast<T>(component)) {
            return castedComponent;
        }
    }

    return nullptr;
}

