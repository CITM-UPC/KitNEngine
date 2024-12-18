//
// Created by Roger on 4/12/2024.
//

#include "Modules/Inspector.h"
#include "Structures/UIWindows.h"
#include <imgui.h>

Inspector::Inspector() : Module("Inspector", false)
{
    
}

Inspector::~Inspector()
{
}

bool Inspector::Init()
{
    Module::Init();
    return true;
}

bool Inspector::Start()
{
    Module::Start();
    return true;
}

bool Inspector::PreUpdate()
{
    _active = _enabled;
    Module::PreUpdate();
    return true;
}

bool Inspector::Update()
{
    Module::Update();
    return true;
}

bool Inspector::LateUpdate()
{
    Module::LateUpdate();
    return true;
}

bool Inspector::EditorUI()
{
    if (!Module::EditorUI()) return true;
    
    ImGui::SetNextWindowPos(ImVec2(20, 220), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);

    //TODO treure codi temporal de debug
    ImGui::Begin("Input Debug");
    ImGui::Text("Is Any Item Active: %s", ImGui::IsAnyItemActive() ? "Yes" : "No");
    ImGui::Text("Is Any Item Focused: %s", ImGui::IsAnyItemFocused() ? "Yes" : "No");
    ImGui::Text("Is Any Item Hovered: %s", ImGui::IsAnyItemHovered() ? "Yes" : "No");
    ImGui::End();
    
    ImGui::SetNextWindowPos(ImVec2(10, 220), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Inspector", &_enabled)) {
        if (GameObject::selectedGameObject != nullptr
            && !GameObject::selectedGameObject->InspectorDisplay())
            return false;
    }
    ImGui::End();
    return true;
}

bool Inspector::CleanUp()
{
    return true;
}
