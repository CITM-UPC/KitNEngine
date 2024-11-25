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


std::vector<GameObjectPtr> GameObject::gameObjects = std::vector<GameObjectPtr>();

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
        if (go->_active && !go->Awake())
            return false;
    }

    for (ComponentPtr& go : components)
    {
        if (go->_active && !go->Awake())
            return false;
    }

    return true;
}

bool GameObject::Start()
{
    if (!Component::Start()) return false;
    
    for (GameObjectPtr& go : children)
    {
        if (go->_active && !go->Start())
            return false;
    }

    for (ComponentPtr& go : components)
    {
        if (go->_active && !go->Start())
            return false;
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
    //Component::InspectorDisplay();
    ImGui::Text("GameObject");
    
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
    // TODO acaba aixo
    std::cerr << "GameObject::RemoveComponent called - Implementation unfinished: Expect errors" << std::endl;
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
    // TODO acaba aixo
    std::cerr << "GameObject::RemoveComponent called - Implementation unfinished: Expect errors" << std::endl;
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

template <std::derived_from<Component> T>
std::shared_ptr<T> GameObject::AddComponentOfType()
{
    static_assert(!std::is_base_of_v<GameObject, T>, "T NO ha derivar de GameObject");

    std::shared_ptr<T> c = std::make_shared<T>(*this);
    components.push_back(c);

    if (_awoken && c->_enabled && !c->_awoken)
    {
        c->Awake();
    }

    return c;
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

