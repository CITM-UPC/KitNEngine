//
// Created by Roger on 02/10/2024.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Component/Component.h"
#include "Component/Transform.h"

#include <memory>
#include <vector>

class MeshRenderer;

// Evita instanciar GameObjects directament, utilitza CreateGameObject() en el seu lloc
class GameObject final: public Component, public std::enable_shared_from_this<GameObject>
{
public: // Static members/functions

    static std::vector<std::shared_ptr<GameObject>> gameObjects;
    static std::shared_ptr<GameObject> selectedGameObject;

    static std::shared_ptr<GameObject> CreateGameObject(std::shared_ptr<GameObject> parent, std::string name = "GameObject");
    static std::shared_ptr<GameObject> GetAsSmartPtr(GameObject* ptr);
    
public:
    
    explicit GameObject(const std::string& name = "GameObject");
    ~GameObject();

    bool Awake() override;
    bool Start() override;
    bool PreUpdate() override;
    bool Update() override;
    bool PostUpdate() override;
    bool InspectorDisplay() override;
    bool CleanUp() override;
    void Render();

    bool IsGameObject() const override { return true; }

    GameObject& SetParent(std::shared_ptr<GameObject> parent);

    [[nodiscard]] std::shared_ptr<GameObject>& GetChild(glm::uint index) { return children.at(index); }
    [[nodiscard]] std::vector<std::shared_ptr<GameObject>>& GetChildren() { return children; }
    [[nodiscard]] TransformPtr GetTransform() { return transform; }

    template <typename T>
    [[nodiscard]] std::shared_ptr<T> GetComponentOfType() const;

    template <typename T, typename... Args>
    requires std::derived_from<T, Component>
    std::shared_ptr<T> AddComponentOfType(Args&&... args);


    // Afegeix el component a la llista d'aquest GameObject
    std::shared_ptr<Component> AddComponent(std::shared_ptr<Component> c);
    void RemoveComponent(std::shared_ptr<Component>& component);

    std::shared_ptr<GameObject> AddChild(std::shared_ptr<GameObject>& g);
    void RemoveChild(const std::shared_ptr<GameObject>& child);

public:

    std::shared_ptr<GameObject> parent;
    
    std::weak_ptr<MeshRenderer> renderer;
    
private:
    TransformPtr transform = std::make_shared<Transform>(this);

private:
    
    

    std::vector<std::shared_ptr<Component>> components;
    std::vector<std::shared_ptr<GameObject>> children;

    friend Transform;
};


template <typename T, typename... Args>
requires std::derived_from<T, Component>
std::shared_ptr<T> GameObject::AddComponentOfType(Args&&... args)
{
    static_assert(!std::is_base_of_v<GameObject, T>, "T NO ha derivar de GameObject");

    std::shared_ptr<T> c = Component::CreateComponentOfType<T>(std::forward<Args>(args)...);
    AddComponent(c);

    return c;
}


template <class T>
std::shared_ptr<T> GameObject::GetComponentOfType() const
{
    static_assert(std::is_base_of_v<Component, T>, "T ha de derivar de Component");
    static_assert(!std::is_base_of_v<GameObject, T>, "T NO ha de ser un GameObject. Utilitza GetChild per obtenir un GameObject");

    for (const std::shared_ptr<Component>& component : components) {
        // Attempt to cast to the desired type and return it if successful
        auto comp = std::dynamic_pointer_cast<T>(component);
        if (comp != nullptr) {
            return comp;
        }
    }

    return nullptr;
}

#endif //GAMEOBJECT_H
