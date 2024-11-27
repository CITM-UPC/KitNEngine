//
// Created by Roger on 10/11/2024.
//

#include "Modules/EntityManager.h"

#include <filesystem>

#include "Modules/Input.h"
#include "Component/GameObject.h"

#include <iostream>
#include <ostream>

#include "Structures/MeshRenderer.h"
#include "Structures/Texture.h"
#include "Utilities/Log.h"

EntityManager::EntityManager() : Module("EntityManager")
{
    Input::OnDropFile.addCB([this](OnDropEventType& path){this->OnDropFile(path);});
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

    GameObject::gameObjects.clear();
    
    return ret && Module::CleanUp();
}

void EntityManager::OnDropFile(OnDropEventType& fileName)
{
    auto extension = std::filesystem::path(fileName).extension().string();
    if (extension == ".fbx")
    {
        MeshRenderer::meshes.clear();
        auto meshes = MeshRenderer::ImportMeshes(fileName.c_str());

        // TODO Guardar la mesh al GameObject a través d'un component
        // GameObjectPtr gameObject = GameObjectPtr(new GameObject(nullptr));
        // for (auto& mesh : meshes)
        // {
        //     auto g = gameObject->AddChild(GameObjectPtr(new GameObject(nullptr)));
        //     g->
        // }
    }
    else if (extension == ".png" || extension == ".PNG"
        || extension == ".JPG" || extension == ".JPEG" || extension == ".jpg" || extension == ".jpeg"
        || extension == ".dds" || extension == ".DDS"
        )
    {
        Texture::textures.clear();
        const auto id = Texture::ImportTexture(fileName.c_str());

        for (auto& mesh : MeshRenderer::meshes)
        {
            mesh->texture_id = id;
        }
    }
}
