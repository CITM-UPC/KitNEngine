//
// Created by Roger on 9/11/2024.
//

#include "Game/Core/Game.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include "Game/Modules/ResourceManager.h"
#include "Game/Modules/Input.h"
#include "Game/Modules/EntityManager.h"


Game::Game()
{    
    // Lib initialization
}

Game::~Game()
{
    
}

bool Game::Init()
{
    // Module creation
    AddModule(input = std::make_shared<Input>());
    AddModule(entities = std::make_shared<EntityManager>());
    AddModule(resources = ResourceManager::getInstance()); // Sol pot haver un gestor de recursos
    
    return true;
}

bool Game::Start()
{
    return App::Start();
}

bool Game::Update()
{
    if(input->GetWindowEvent(WE_QUIT) == true) return false;

    // Main loop
    if (!App::Update()) return false;

    // Render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    entities->Render();
    
    return true;
}

bool Game::CleanUp()
{
    // Clean up everything and stop... stuff?
    for (auto module = modules.begin(); module != modules.end(); ++module)
    {
        if (!module->operator->()->CleanUp())
            return false;
    }
    return true;
}