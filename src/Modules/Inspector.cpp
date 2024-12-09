//
// Created by Roger on 4/12/2024.
//

#include "Modules/Inspector.h"
#include "Structures/UIWindows.h"
#include <imgui.h>

Inspector::Inspector() : Module("Inspector")
{
    
}

Inspector::~Inspector()
{
}

bool Inspector::Init()
{
    return Module::Init();
}

bool Inspector::Start()
{
    return Module::Start();
}

bool Inspector::PreUpdate()
{
    return Module::PreUpdate();
}

bool Inspector::Update()
{
    return Module::Update();
}

bool Inspector::LateUpdate()
{
    return Module::LateUpdate();
}

bool Inspector::EditorUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 220), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Inspector", &_active)) {
        if (GameObject::selectedGameObject != nullptr)
        GameObject::selectedGameObject->InspectorDisplay();
        


    }
    ImGui::End();
    return Module::EditorUI();
}

bool Inspector::CleanUp()
{
    return Module::CleanUp();
}
