//
// Created by Roger on 6/11/2024.
//

#include "Game/Component/GameObject.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

#include "Game/Component/MeshRenderer.h"
#include "Editor/Misc/UIWindows.h"


std::vector<std::shared_ptr<GameObject>> GameObject::gameObjects = std::vector<std::shared_ptr<GameObject>>();

std::shared_ptr<GameObject> GameObject::selectedGameObject = nullptr;

std::shared_ptr<GameObject> GameObject::CreateGameObject(std::shared_ptr<GameObject> parent, std::string name)
{
    auto ret = std::make_shared<GameObject>(name);
    
    ret->gameObject = ret; // Guarda referencia de smart pointer per acces rapid des d'altres scripts
    if (parent == nullptr)
        gameObjects.push_back(ret);
    else
        parent->AddChild(ret);

    return ret;
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
    if (!Component::PreUpdate()) return true;
    
    for (std::shared_ptr<GameObject>& go : children)
    {
        if (!go->PreUpdate())
            return false;
    }

    for (std::shared_ptr<Component>& c : components)
    {
        if (!c->PreUpdate())
            return false;
    }

    return true;
}

bool GameObject::Update()
{
    if (!Component::Update()) return true;
    
    for (std::shared_ptr<GameObject>& go : children)
    {
        if (!go->Update())
            return false;
    }

    for (std::shared_ptr<Component>& go : components)
    {
        if (!go->Update())
            return false;
    }

    return true;
}

bool GameObject::PostUpdate()
{
    if (!Component::PostUpdate()) return true;

    for (std::shared_ptr<GameObject>& go : children)
    {
        if (!go->PostUpdate())
            return false;
    }

    for (std::shared_ptr<Component>& go : components)
    {
        if (!go->PostUpdate())
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

    ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_EnterReturnsTrue;
    
    Component::InspectorDisplay();
    ImGui::Checkbox(("GameObject##Enable"), &_enabled);

    ImGui::BeginGroup();
    ImGui::Text("Nom:");
    ImGui::SameLine();
    if (ImGui::InputText("##name", &_name, inputFlags))
        AddLogMessage("changed to %s", _name.c_str());
    transform->InspectorDisplay(inputFlags);
    
    ImGui::EndGroup();
    
    for (std::shared_ptr<Component>& component : components)
    {
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2);
        ImGui::PushID(component.get());
        component->InspectorDisplay();
        ImGui::PopID();
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

std::shared_ptr<GameObject> GameObject::AddChild(std::shared_ptr<GameObject>& g)
{
    if (g.get() != this && g->parent.get() != this)
    {
        children.push_back(g);
        g->parent = GetGameObject();
    }
    
    return g;
}

void GameObject::RemoveChild(const std::shared_ptr<GameObject>& child)
{
    auto pred = [child](const std::shared_ptr<GameObject>& go){return go == child;};
    std::erase_if(children, pred);
}

void GameObject::Render()
{
    if (!_active) return;

    if (auto r = renderer.lock(); r != nullptr) r->Render();

    for (std::shared_ptr<GameObject>& go : children)
    {
        go->Render();
    }
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

