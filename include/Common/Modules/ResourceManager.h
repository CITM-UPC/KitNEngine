//
// Created by Roger on 25/12/2024.
//

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <chrono>
#include <map>
#include "Modules/Module.h"

class Texture;
class Shader;
class KMesh;

class ResourceManager : Module {
public:
    ~ResourceManager() override;
    bool Init() override;
    bool Start() override;
    bool PreUpdate() override;
    bool Update() override;
    bool LateUpdate() override;
    bool EditorUI() override;
    bool CleanUp() override;

    void LoadMesh(std::string path);
    void LoadTexture(std::string path);
    void LoadShader(std::string path);

    std::shared_ptr<KMesh> GetMesh(std::string& name);
    std::shared_ptr<Texture> GetTexture(std::string& name);
    std::shared_ptr<Shader> GetShader(std::string& name);
    
    void UnloadMesh(std::string& name);
    void UnloadTexture(std::string& name);
    void UnloadShader(std::string& name);

    // Elimina tots els recursos de memoria un cop paren de ser utilitzats (treu totes les referencies del gestor de recursos)
    void UnloadAll();

private:
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::seconds> lastUpdate;
    std::chrono::duration<float> timeBetweenUpdates = std::chrono::seconds(5); // Temps entre actualitzacions de recursos d'editor

    std::map<std::string, std::shared_ptr<KMesh>> meshes;
    std::map<std::string, std::shared_ptr<Texture>> textures;
    std::map<std::string, std::shared_ptr<Shader>> shaders;
};



#endif //RESOURCEMANAGER_H
