//
// Created by Roger on 6/11/2024.
//

#include "Component/GameObject.h"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <memory>
#include <memory>
#include <ostream>
#include <stdexcept>

#include <utility>

#include "glm/gtc/type_ptr.hpp"


std::vector<std::shared_ptr<GameObject>> GameObject::gameObjects = std::vector<std::shared_ptr<GameObject>>();

std::shared_ptr<GameObject> GameObject::selectedGameObject = nullptr;

std::shared_ptr<GameObject> GameObject::CreateGameObject(std::shared_ptr<GameObject> parent, std::string name)
{
    auto ret = std::make_shared<GameObject>();
    
    ret->gameObject = ret; // Guarda referencia de smart pointer per acces rapid des d'altres scripts
    if (parent == nullptr)
        gameObjects.push_back(ret);
    else
        parent->AddChild(ret);

    return ret;
}

// OBSOLET, utilitza GetGameObject sobre el punter de GameObject
// Cerca un smart pointer corresponent al punter passat per parametre
std::shared_ptr<GameObject> GameObject::GetAsSmartPtr(GameObject* ptr)
{
    if (ptr == nullptr) return nullptr;
    
    if (ptr->parent != nullptr)
    {
        auto list = ptr->parent->GetChildren();
        for (auto it = list.begin(); it != list.end(); it++)
        {
            if (it->get() == ptr) return *it;
        }
    }
    else
    {
        for (auto it = gameObjects.begin(); it != gameObjects.end(); it++)
        {
            if (it->get() == ptr) return *it;
        }
    }
    // Si el codi arriba aqui, has fet alguna cosa MOLT malament (possiblement intentant agafar el smart pointer de la camera d'editor
    throw std::runtime_error("GameObject::GetAsSmartPtr: gameobject not in main hierarchy and has no parent");
}

GameObject::GameObject(const std::string& name) : Component(name)
{
}

GameObject::~GameObject()
{
    CleanUp();
}

bool GameObject::Awake()
{    
    for (std::shared_ptr<GameObject>& go : children)
    {
        if (go->_active) go->Awake();
    }

    for (std::shared_ptr<Component>& go : components)
    {
        if (go->_active) go->Awake();
    }

    return true;
}

bool GameObject::Start()
{
    // Si l'objecte esta desactivat surt abans
    if (!Component::Start()) return true;
    
    for (std::shared_ptr<GameObject>& go : children)
    {
        if (go->_active) go->Start();
    }

    for (std::shared_ptr<Component>& go : components)
    {
        if (go->_active) go->Start();
    }

    return true;
}

bool GameObject::PreUpdate()
{
    if (!Component::PreUpdate()) return false;
    
    for (std::shared_ptr<GameObject>& go : children)
    {
        if (go->_active && !go->PreUpdate())
            return false;
    }

    for (std::shared_ptr<Component>& go : components)
    {
        if (go->_active && !go->PreUpdate())
            return false;
    }

    return true;
}

bool GameObject::Update()
{
    if (!Component::Update()) return false;
    
    for (std::shared_ptr<GameObject>& go : children)
    {
        if (go->_active && !go->Update())
            return false;
    }

    for (std::shared_ptr<Component>& go : components)
    {
        if (go->_active && !go->Update())
            return false;
    }

    return true;
}

bool GameObject::PostUpdate()
{
    if (!Component::PostUpdate()) return false;

    for (std::shared_ptr<GameObject>& go : children)
    {
        if (go->_active && !go->PostUpdate())
            return false;
    }

    for (std::shared_ptr<Component>& go : components)
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
    ImGui::InputText("Nom:", &_name);
    //TODO trobar com posar 3 valors editables a la mateixa fila
    /*if (ImGui::SliderFloat3("Position: ", pos,(-INFINITY),(INFINITY),"%.6f"))
    {
        transform->SetPosition(glm::vec3(pos[0], pos[1], pos[2]));
    }*/
    ImGui::EndGroup();
    
    for (std::shared_ptr<Component>& component : components)
    {
        component->InspectorDisplay();
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

GameObject& GameObject::SetParent(std::shared_ptr<GameObject> parent)
{
    std::shared_ptr<GameObject> ptr = GetGameObject();
    if (parent != nullptr)
        parent->RemoveChild(ptr);
    parent->AddChild(ptr);
    //this->parent = std::move(parent);
    
    return *this;
}

void GameObject::RemoveComponent(std::shared_ptr<Component>& component)
{
    auto pred = [component](const std::shared_ptr<Component>& c){return c == component;};
    std::erase_if(components, pred);
}

GameObject& GameObject::AddChild(std::shared_ptr<GameObject>& g)
{
    if (g.get() != this && g->parent.get() != this)
    {
        children.push_back(g);
        g->parent = GetGameObject();
    }
    
    return *g;
}

void GameObject::RemoveChild(const std::shared_ptr<GameObject>& child)
{
    auto pred = [child](const std::shared_ptr<GameObject>& go){return go == child;};
    std::erase_if(children, pred);
}

template <class T>
T& GameObject::GetComponentOfType() const
{
    static_assert(std::is_base_of_v<Component, T>, "T ha de derivar de Component");
    static_assert(!std::is_base_of_v<GameObject, T>, "T NO ha de ser un GameObject. Utilitza GetChild per obtenir un GameObject");

    for (const std::shared_ptr<Component>& component : components) {
        // Attempt to cast to the desired type and return it if successful
        auto& comp = dynamic_cast<Component&>(*component);
        if (T& castedComponent = dynamic_cast<T>(comp)) {
            return castedComponent;
        }
    }

    return nullptr;
}

std::shared_ptr<Component> GameObject::AddComponent(std::shared_ptr<Component> c)
{
    if (dynamic_pointer_cast<GameObject>(c) != nullptr)
        throw std::logic_error("Component must not be a GameObject");
    
    auto thisptr = gameObject.lock();
    auto componentParent = c->gameObject.lock();
    if (componentParent != nullptr && componentParent != thisptr)
    {
        componentParent->RemoveComponent(c);
    }
    
    c->gameObject = thisptr;
    components.push_back(c);

    if (_awoken && c->_enabled && !c->_awoken)
    {
        c->Awake();
    }
    
    return c;
}

