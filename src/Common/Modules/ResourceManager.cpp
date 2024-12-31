//
// Created by Roger on 25/12/2024.
//

#include "Game/Modules/ResourceManager.h"
#include "Game/Resources/Resource.h"

#include "Game/Utilities/Time.h"
std::weak_ptr<ResourceManager> ResourceManager::rm_instance;

std::shared_ptr<ResourceManager> ResourceManager::getInstance()
{
    std::shared_ptr<ResourceManager> ret;
    if (rm_instance.expired())
        ret = std::make_shared<ResourceManager>();
    else
        ret = rm_instance.lock();
    return ret;
}

ResourceManager::ResourceManager() : Module("resourcemanager")
{
}

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

    // Elimina recursos que no estiguin en us
    auto pred = [](auto& r) -> bool {return !r.second->InUse();};

    std::erase_if(resourceMap, pred);
    
    return true;
}

bool ResourceManager::EditorUI()
{
    return Module::EditorUI();
}

bool ResourceManager::CleanUp()
{
    resourceMap.clear();
    return true;
}

std::shared_ptr<Resource> ResourceManager::LoadMesh(std::string path)
{
    AddLogMessage("Loading mesh from %s\n", path);
    throw std::logic_error("Unfinished function ResourceManager::LoadMesh");
}

std::shared_ptr<Resource> ResourceManager::LoadTexture(std::string path)
{
    AddLogMessage("Loading texture from %s\n", path);
    throw std::logic_error("Unfinished function ResourceManager::LoadTexture");
}

std::shared_ptr<Resource> ResourceManager::LoadShader(std::string path)
{
    AddLogMessage("Loading shader from %s\n", path);
    throw std::logic_error("Unfinished function ResourceManager::LoadShader");
}

template <class T> requires std::derived_from<T, Resource>
void ResourceManager::Unload(std::string& name)
{
    if (resourceMap.find(name) != resourceMap.end())
        resourceMap.erase(name);
}

void ResourceManager::UnloadAll()
{
    resourceMap.clear();
}
