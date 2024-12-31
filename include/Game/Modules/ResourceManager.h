//
// Created by Roger on 25/12/2024.
//

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include "Module.h"
#include <chrono>
#include <map>

#include "Game/Resources/Resource.h"
#include "Libs/stduuid/uuid.h"
#include "Editor/Misc//UIWindows.h"

class Resource;
class KMesh;
class Texture;
class Shader;

class ResourceManager : public Module {
public:
    static std::weak_ptr<ResourceManager> rm_instance;
    //Singleton
    static std::shared_ptr<ResourceManager> getInstance();

    ResourceManager();
    ~ResourceManager() override;
    bool Init() override;
    bool Start() override;
    bool PreUpdate() override;
    bool Update() override;
    bool LateUpdate() override;
    bool EditorUI() override;
    bool CleanUp() override;

    std::shared_ptr<Resource> LoadMesh(std::string path);
    std::shared_ptr<Resource> LoadTexture(std::string path);
    std::shared_ptr<Resource> LoadShader(std::string path);

    // Retorna un punter al recurs demanat
    template <class T>
    requires std::derived_from<T, Resource>
    std::shared_ptr<T> Get(std::string& name);

    // Retorna un punter al recurs demanat (uuid)
    template <class T>
    requires std::derived_from<T, Resource>
    std::shared_ptr<T> Get(uuids::uuid& uuid);

    // Treu del resourceMap la referencia de l'element especificat
    template <class T>
    requires std::derived_from<T, Resource>
    void Unload(std::string& name);

    // Elimina tots els recursos de memoria un cop paren de ser utilitzats (treu totes les referencies del gestor de recursos)
    // Els recursos s'eliminaran de la memoria quan ja no estiguin en us
    void UnloadAll();

private:
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::seconds> lastUpdate;
    std::chrono::duration<float> timeBetweenUpdates = std::chrono::seconds(5); // Temps entre actualitzacions de recursos d'editor

    std::map<std::string, std::shared_ptr<Resource>> resourceMap;
};

template <class T> requires std::derived_from<T, Resource>
std::shared_ptr<T> ResourceManager::Get(std::string& name)
{
    if (resourceMap.contains(name))
        return std::dynamic_pointer_cast<T>(resourceMap[name]);
    
    AddLogMessage("Resource %s not found", name.c_str());
    
    if (std::is_same_v<T,KMesh>)
    {
        return LoadMesh(name);
    }
    if (std::is_same_v<T, Texture>)
    {
        return LoadTexture(name);
    }
    if (std::is_same_v<T, Shader>)
    {
        return LoadShader(name);
    }
    AddLogMessage("Unknown resource type");
    return nullptr;
}

template <class T> requires std::derived_from<T, Resource>
std::shared_ptr<T> ResourceManager::Get(uuids::uuid& uuid)
{
    for (auto it = resourceMap.begin(); it != resourceMap.end(); it++)
    {
        if (it->second->r_uuid == uuid)
        {
            return it->second;
        }
    }
    AddLogMessage("UUID no encontrada");
    return nullptr;
}


#endif //RESOURCEMANAGER_H
