//
// Created by Roger on 11/12/2024.
//

#include "Editor/Core/Editor.h"

//Libs
#include <memory>

//Modules
#include "Editor/Modules/Inspector.h"

//Components
#include "Game/Component/GameObject.h"
#include "Game/Component/Camera.h"

Editor::Editor()
{
    
}

bool Editor::Init()
{
    AddModule(inspector = std::make_shared<Inspector>());

    editorObject = std::make_shared<GameObject>("EditorObject"); // No CreateGameObject perque es un objecte independent
    editorObject->gameObject = editorObject; // Guarda referencia de smart pointer per acces rapid des d'altres scripts
    editorCamera = editorObject->AddComponentOfType<Camera>();

    return true;
}

bool Editor::Start()
{
    editorCamera->Awake();
    editorCamera->Start();
    return true;
}

bool Editor::Update()
{

    if (!App::Update()) return false;

    for (std::shared_ptr<Module>& m : modules)
    {
        if (!m->EditorUI()) return false;
    }

    if (!editorObject->Update()) return false;
    
    return true;
}

bool Editor::CleanUp()
{
    return true;
}

Editor::~Editor()
{
    
}
