//
// Created by Roger on 02/10/2024.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Component.h"
#include "Transform.h"

#include <memory>
#include <vector>

class GameObject final: public Component
{
public: // Static members/functions

    static std::vector<GameObjectPtr> gameObjects;
    
public:
    
    GameObject(GameObjectPtr go);
    ~GameObject();

    bool Awake() override;
    bool Start() override;
    bool PreUpdate() override;
    bool Update() override;
    bool PostUpdate() override;
    bool InspectorDisplay() override;
    bool CleanUp() override;

    bool IsGameObject() const override { return true; }

    GameObject& SetParent(GameObject* parent);
    
    [[nodiscard]] GameObjectPtr& GetChild(glm::uint index) { return children.at(index); }
    [[nodiscard]] std::vector<GameObjectPtr>& GetChildren() { return children; }
    [[nodiscard]] const TransformPtr GetTransform() { return transform; }

    template <typename T>
    [[nodiscard]] T& GetComponentOfType() const;

    template <std::derived_from<Component> T>
    std::shared_ptr<T> AddComponentOfType();
    
    
    // Afegeix el component a la llista d'aquest GameObject
    ComponentPtr AddComponent(ComponentPtr& c);
    void RemoveComponent(Component* component);

    GameObject& AddChild(GameObjectPtr& g);
    GameObject& AddChild(GameObject* g);
    void RemoveChild(const GameObject* child);


public:

    GameObjectPtr parent;

private:

    GameObject* gameObject = this;
    
    TransformPtr transform = std::make_shared<Transform>(this);

    std::vector<ComponentPtr> components;
    std::vector<GameObjectPtr> children;

    friend Transform;
};

#endif //GAMEOBJECT_H
