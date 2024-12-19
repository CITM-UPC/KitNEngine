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

EntityManager::~EntityManager() = default;

bool EntityManager::Init()
{
    // TODO canviar LOG() per implementació pròpia
    LOG("Loading Entity Manager");

    if (!Module::Init()) return false;

    for (std::shared_ptr<GameObject>& gameObject : GameObject::gameObjects)
    {
        gameObject->Awake();
    }

    _awoken = true;

    return true;
}

bool EntityManager::Start()
{
    if (!Module::Start()) return false;

    for (std::shared_ptr<GameObject>& gameObject : GameObject::gameObjects)
    {
        gameObject->Start();
    }

    _started = true;

    return true;
}

bool EntityManager::PreUpdate()
{
    if(!Module::PreUpdate()) return true;

    for (std::shared_ptr<GameObject>& gameObject : GameObject::gameObjects)
    {
        gameObject->PreUpdate();
    }

    return true;
}

bool EntityManager::Update()
{
    if (!Module::Update()) return true;
    
    for (std::shared_ptr<GameObject>& gameObject : GameObject::gameObjects)
    {
        gameObject->Update();
    }

    return true;
}

bool EntityManager::LateUpdate()
{
    if (!Module::LateUpdate()) return true;

    for (std::shared_ptr<GameObject>& gameObject : GameObject::gameObjects)
    {
        if (!gameObject->PostUpdate())
            return false;
    }

    return true;
}

bool EntityManager::CleanUp()
{
    bool ret = true;
    for (std::shared_ptr<GameObject>& gameObject : GameObject::gameObjects)
    {
        ret &= gameObject->CleanUp();
    }

    GameObject::gameObjects.clear();
    
    return ret;
}

bool EntityManager::Render()
{
    for (std::shared_ptr<GameObject>& gameObject : GameObject::gameObjects)
    {
        gameObject->Render();
    }
    
    /*for ( std::shared_ptr<MeshRenderer>& renderer : MeshRenderer::renderers)
    {
        renderer->Render();
    }*/
    return true;
}

void EntityManager::OnDropFile(OnDropEventType& fileName)
{
    // TODO Deprecar quan tinguem recursos interns de l'editor, o com a minim fer que no elimini tots els objectes existents
    auto extension = std::filesystem::path(fileName).extension().string();
    if (extension == ".fbx")
    {
        MeshRenderer::renderers.clear();
        auto meshes = MeshRenderer::ImportMeshes(fileName.c_str());

    }
    else if (extension == ".png" || extension == ".PNG"
        || extension == ".JPG" || extension == ".JPEG" || extension == ".jpg" || extension == ".jpeg"
        || extension == ".dds" || extension == ".DDS"
        )
    {
        Texture::textures.clear();
        const auto id = Texture::ImportTexture(fileName.c_str());

        for (auto& mesh : MeshRenderer::renderers)
        {
            mesh->texture_id = id;
        }
    }
}
