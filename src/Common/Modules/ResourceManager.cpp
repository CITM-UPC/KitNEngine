//
// Created by Roger on 25/12/2024.
//

#include "Common/Modules/ResourceManager.h"
#include "Common/Resources/KMesh.h"

#include "Utilities/Time.h"

ResourceManager::~ResourceManager()
{
    
}

bool ResourceManager::Init()
{
    return Module::Init();
}

bool ResourceManager::Start()
{
    return Module::Start();
}

bool ResourceManager::PreUpdate()
{
    return Module::PreUpdate();
}

bool ResourceManager::Update()
{
    return Module::Update();
}

bool ResourceManager::LateUpdate()
{
    if (!Module::LateUpdate()) return true;

    
    //if (Time::since(lastUpdate) < timeBetweenUpdates) return true;
    //lastUpdate = Time::GetTime<std::chrono::steady_clock, std::chrono::seconds>();

    // TODO comprovar i eliminar recursos que no estiguin en us
   // auto pred = [](ResourceHandle& r) -> r.
    
    return true;
}

bool ResourceManager::EditorUI()
{
    return Module::EditorUI();
}

bool ResourceManager::CleanUp()
{
    return true;
}

void ResourceManager::LoadMesh(std::string path)
{
    throw std::logic_error("Not implemented");
}

void ResourceManager::LoadTexture(std::string path)
{
    throw std::logic_error("Not implemented");
}

void ResourceManager::LoadShader(std::string path)
{
    throw std::logic_error("Not implemented");
}

std::shared_ptr<KMesh> ResourceManager::GetMesh(std::string& name)
{
    throw std::logic_error("Not implemented");
}

std::shared_ptr<Texture> ResourceManager::GetTexture(std::string& name)
{
    throw std::logic_error("Not implemented");
}

std::shared_ptr<Shader> ResourceManager::GetShader(std::string& name)
{
    throw std::logic_error("Not implemented");
}

void ResourceManager::UnloadMesh(std::string& name)
{
    throw std::logic_error("Not implemented");
}

void ResourceManager::UnloadTexture(std::string& name)
{
    throw std::logic_error("Not implemented");
}

void ResourceManager::UnloadShader(std::string& name)
{
    throw std::logic_error("Not implemented");
}

void ResourceManager::UnloadAll()
{
    throw std::logic_error("Not implemented");
}
