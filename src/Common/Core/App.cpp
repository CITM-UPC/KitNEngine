//
// Created by Roger on 9/11/2024.
//

#include "Common/Core/App.h"

#include <stdexcept>

App::App()
{    

}

App::~App()
{
    
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
    // Main loop
    
    for (auto module = modules.begin(); module != modules.end(); ++module)
    {
        if (!(module->operator->()->PreUpdate())) // DUBTE: No em deixa accedir directament a la funció de l'objecte
            return false;
    }
    
    for (auto module = modules.begin(); module != modules.end(); ++module)
    {
        if (!(module->operator->()->Update()))
            return false;
    }
    
    for (auto module = modules.begin(); module != modules.end(); ++module)
    {
        if (!(module->operator->()->LateUpdate()))
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

ModulePtr App::GetModule(const std::string& name) const
{
    for (auto module : modules)
    {
        if (module->GetName() == name) return module;
    }
    return nullptr;
}
