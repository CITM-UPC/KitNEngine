//
// Created by Roger on 9/11/2024.
//

#include "Core/App.h"


App::App()
{    
    // Lib initialization
}

App::~App()
{
    
}

bool App::Init()
{
    // Module creation
    AddModule(input = std::make_shared<Input>());
    
    return true;
}

bool App::Start()
{
    // Module initialization
    for (auto module = modules.begin(); module != modules.end(); ++module)
    {
        if (!module->operator->()->Init()) 
            return false;
    }

    for (auto module = modules.begin(); module != modules.end(); ++module)
    {
        if (!module->operator->()->Start())
            return false;
    }
        
    return true;
}

bool App::Update()
{
    if(input->GetWindowEvent(WE_QUIT) == true)
        return false;
    
    // Main loop
    
    for (auto module = modules.begin(); module != modules.end(); ++module)
    {
        if (!module->operator->()->PreUpdate()) // DUBTE: No em deixa accedir directament a la funció de l'objecte
            return false;
    }
    
    for (auto module = modules.begin(); module != modules.end(); ++module)
    {
        if (!module->operator->()->Update())
         return false;
    }
    
    for (auto module = modules.begin(); module != modules.end(); ++module)
    {
        if (!module->operator->()->PostUpdate())
            return false;
    }
    
    return true;
}

bool App::CleanUp()
{
    // Clean up everything and stop... stuff?
    for (auto module = modules.begin(); module != modules.end(); ++module)
    {
        if (!module->operator->()->CleanUp())
            return false;
    }
    return true;
}

bool App::AddModule(const ModulePtr& module)
{
    modules.push_back(module);
    return true;
}