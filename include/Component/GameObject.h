//
// Created by Roger on 02/10/2024.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Component.h"
#include "Transform.h"

#include <memory>
#include <vector>

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

    bool IsGameObject() const override { return true; }

    GameObject& SetParent(std::shared_ptr<GameObject> parent);
    
    [[nodiscard]] std::shared_ptr<GameObject>& GetChild(glm::uint index) { return children.at(index); }
    [[nodiscard]] std::vector<std::shared_ptr<GameObject>>& GetChildren() { return children; }
    [[nodiscard]] TransformPtr GetTransform() { return transform; }

    template <typename T>
    [[nodiscard]] T& GetComponentOfType() const;

    template <typename T, typename... Args>
    requires std::derived_from<T, Component>
    std::shared_ptr<T> AddComponentOfType(Args&&... args);
    
    
    // Afegeix el component a la llista d'aquest GameObject
    std::shared_ptr<Component> AddComponent(std::shared_ptr<Component> c);
    void RemoveComponent(std::shared_ptr<Component>& component);

    GameObject& AddChild(std::shared_ptr<GameObject>& g);
    void RemoveChild(const std::shared_ptr<GameObject>& child);


public:

    std::shared_ptr<GameObject> parent;
    

private:
    
    TransformPtr transform = std::make_shared<Transform>(this);

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

#endif //GAMEOBJECT_H
