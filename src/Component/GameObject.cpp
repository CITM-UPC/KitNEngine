//
// Created by Roger on 6/11/2024.
//

#include "Component/GameObject.h"

#include <imgui.h>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>

#include <utility>

#include "glm/gtc/type_ptr.hpp"


std::vector<GameObjectPtr> GameObject::gameObjects = std::vector<GameObjectPtr>();

GameObjectPtr GameObject::selectedGameObject = nullptr;

GameObjectPtr GameObject::CreateGameObject(GameObjectPtr& parent)
{
    GameObjectPtr ret;
    if (parent == nullptr)
        ret = gameObjects.emplace_back(ret);
    else
        parent->AddChild(ret);

    return ret;
}

GameObject::GameObject(GameObjectPtr go) : parent(std::move(go))
{
}

GameObject::~GameObject()
{
    CleanUp();
}

bool GameObject::Awake()
{    
    for (GameObjectPtr& go : children)
    {
        if (go->_active) go->Awake();
    }

    for (ComponentPtr& go : components)
    {
        if (go->_active) go->Awake();
    }

    return true;
}

bool GameObject::Start()
{
    // Si l'objecte esta desactivat surt abans
    if (!Component::Start()) return true;
    
    for (GameObjectPtr& go : children)
    {
        if (go->_active) go->Start();
    }

    for (ComponentPtr& go : components)
    {
        if (go->_active) go->Start();
    }

    return true;
}

bool GameObject::PreUpdate()
{
    if (!Component::PreUpdate()) return false;
    
    for (GameObjectPtr& go : children)
    {
        if (go->_active && !go->PreUpdate())
            return false;
    }

    for (ComponentPtr& go : components)
    {
        if (go->_active && !go->PreUpdate())
            return false;
    }

    return true;
}

bool GameObject::Update()
{
    if (!Component::Update()) return false;
    
    for (GameObjectPtr& go : children)
    {
        if (go->_active && !go->Update())
            return false;
    }

    for (ComponentPtr& go : components)
    {
        if (go->_active && !go->Update())
            return false;
    }

    return true;
}

bool GameObject::PostUpdate()
{
    if (!Component::PostUpdate()) return false;

    for (GameObjectPtr& go : children)
    {
        if (go->_active && !go->PostUpdate())
            return false;
    }

    for (ComponentPtr& go : components)
    {
        if (go->_active && !go->PostUpdate())
            return false;
    }

    return true;
}

bool GameObject::InspectorDisplay()
{
    float pos[3];
    glm::vec3 position = transform->GetPosition();
    pos[0] = position.x;
    pos[1] = position.y;
    pos[2] = position.z;
    
    Component::InspectorDisplay();
    ImGui::Text("GameObject");

    ImGui::BeginGroup();
    ImGui::Text(_name.c_str());
    if (ImGui::SliderFloat3("Position: ", pos,-INFINITY,INFINITY,"%.6f"))
    {
        transform->SetPosition(glm::vec3(pos[0], pos[1], pos[2]));
    }
    
    for (ComponentPtr& component : components)
    {
        if (!component->InspectorDisplay()) return false;
    }
    
    return true;
}

bool GameObject::CleanUp()
{
    bool ret = Component::CleanUp();

    components.clear();
    children.clear();

    return ret;
}

GameObject& GameObject::SetParent(GameObject* parent)
{
    this->parent = std::make_shared<GameObject>(*parent);
    
    return *this;
}

void GameObject::RemoveComponent(Component* component)
{
    // TODO prova aixo
    std::cerr << "GameObject::RemoveComponent called - Implementation untested: Expect errors" << std::endl;
    auto pred = [component](const ComponentPtr& c){return c.get() == component;};
    std::erase_if(components, pred);
}

GameObject& GameObject::AddChild(GameObjectPtr& g)
{
    return AddChild(g.get());
}

GameObject& GameObject::AddChild(GameObject* g)
{
    if (g != this && g->parent.get() != this)
    {
        GameObjectPtr ptr = std::make_shared<GameObject>(*g);
        children.push_back(ptr);
    }
    
    return *this;
}

void GameObject::RemoveChild(const GameObject* child)
{
    // TODO prova aixo
    std::cerr << "GameObject::RemoveChild called - Implementation untested: Expect errors" << std::endl;
    auto pred = [child](const GameObjectPtr& go){return go.get() == child;};
    std::erase_if(children, pred);
}

template <class T>
T& GameObject::GetComponentOfType() const
{
    static_assert(std::is_base_of_v<Component, T>, "T ha de derivar de Component");
    static_assert(!std::is_base_of_v<GameObject, T>, "T NO ha de ser un GameObject. Utilitza GetChild per obtenir un GameObject");

    for (const ComponentPtr& component : components) {
        // Attempt to cast to the desired type and return it if successful
        auto& comp = dynamic_cast<Component&>(*component);
        if (T& castedComponent = dynamic_cast<T>(comp)) {
            return castedComponent;
        }
    }

    return nullptr;
}

ComponentPtr GameObject::AddComponent(ComponentPtr& c)
{
    if (dynamic_pointer_cast<GameObject>(c) != nullptr)
        throw std::logic_error("Component must not be a GameObject");
    
    
    if (c->gameObject != this)
        Component::SetGameObject(c, this);

    components.push_back(c);

    if (_awoken && c->_enabled && !c->_awoken)
    {
        c->Awake();
    }
    
    return c;
}

