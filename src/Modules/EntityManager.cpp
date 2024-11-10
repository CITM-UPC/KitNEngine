//
// Created by Roger on 10/11/2024.
//

#include "Modules/EntityManager.h"

#include <stdexcept>

#include "Utilities/Log.h"

EntityManager::EntityManager() : Module("EntityManager")
{
}

EntityManager::~EntityManager()
{
}

bool EntityManager::Init()
{
    // TODO canviar LOG() per implementació pròpia
    LOG("Loading Entity Manager", 0);

    if (!Module::Init()) return false;

    for (GameObjectPtr& gameObject : GameObject::gameObjects)
        if (!gameObject->Awake()) return false;

    _awoken = true;

    return true;
}

bool EntityManager::Start()
{
    if (!Module::Start()) return false;

    for (GameObjectPtr& gameObject : GameObject::gameObjects)
    {
        if (!gameObject->Start()) return false;
    }

    _started = true;

    return true;
}

bool EntityManager::PreUpdate()
{
    if(!Module::PreUpdate()) return false;

    for (GameObjectPtr& gameObject : GameObject::gameObjects)
    {
        if (!gameObject->PreUpdate()) return false;
    }

    return true;
}

bool EntityManager::Update()
{
    if (!Module::Update()) return false;
    
    for (GameObjectPtr& gameObject : GameObject::gameObjects)
    {
        if (!gameObject->Update())
            return false;
    }

    return true;
}

bool EntityManager::PostUpdate()
{
    if (!Module::PostUpdate()) return false;

    for (GameObjectPtr& gameObject : GameObject::gameObjects)
    {
        if (!gameObject->PostUpdate())
            return false;
    }

    return true;
}

bool EntityManager::CleanUp()
{
    bool ret = true;
    for (GameObjectPtr& gameObject : GameObject::gameObjects)
    {
        ret &= gameObject->CleanUp();
    }
    
    return ret && Module::CleanUp();
}