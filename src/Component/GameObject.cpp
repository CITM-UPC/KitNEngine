//
// Created by Roger on 6/11/2024.
//

#include "Component/GameObject.h"

#include <imgui.h>

GameObject::GameObject()
{
}

void GameObject::InspectorDisplay()
{
    Component::InspectorDisplay();
    ImGui::Text("GameObject");
    for (ComponentPtr& component : components)
        component->InspectorDisplay();
}

template <typename T>
std::shared_ptr<T> GameObject::GetComponentOfType() const
{
    static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
    static_assert(!std::is_base_of_v<GameObject, T>, "T must NOT be a GameObject. Use GetChild instead");

    for (const ComponentPtr& component : components) {
        // Attempt to cast to the desired type and return it if successful
        if (std::shared_ptr<T> castedComponent = std::dynamic_pointer_cast<T>(component)) {
            return castedComponent;
        }
    }

    return nullptr;
}

