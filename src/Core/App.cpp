//
// Created by Roger on 9/11/2024.
//

#include "Core/App.h"

#include <stdexcept>

//AppPtr App::instance(new App());

App::App()
{
    // Lib initialization
}

// Using static magic only one instance of the class will be created
// https://stackoverflow.com/questions/8102125/is-local-static-variable-initialization-thread-safe-in-c11
App& App::GetInstance()
{
    static App instance;
    return instance;
}

bool App::Init()
{
    // Module creation
    input = std::make_shared<Input>();
    return true;
}

bool App::Start()
{
    // Module initialization
    for (auto module = modules.begin(); module != modules.end(); module++)
    {
        if (!module->operator->()->Init()) 
            return false;
    }

    for (auto module = modules.begin(); module != modules.end(); module++)
    {
        if (!module->operator->()->Start())
            return false;
    }
        
    return true;
}

bool App::Update()
{
    // Main loop
    for (auto module = modules.begin(); module != modules.end(); module++)
    {
        if (!module->operator->()->Update()) // DUBTE: No em deixa accedir directament a la funció de l'objecte
         return false;
    }
    return true;
}

bool App::CleanUp()
{
    // Clean up everything and stop... stuff?
    for (auto module = modules.begin(); module != modules.end(); module++)
    {
        if (!module->operator->()->CleanUp())
            return false;
    }
    return true;
}
