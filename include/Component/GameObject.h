//
// Created by Roger on 02/10/2024.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Component.h"
#include "Transform.h"

#include <memory>
#include <vector>

class GameObject : public Component
{
public: // Static members/functions

    static std::vector<GameObjectPtr> gameObjects;
    
public:
    
    GameObject(GameObjectPtr& go);
    ~GameObject() = default;

    void Awake() override;
    void Start() override;
    void PreUpdate() override;
    void Update() override;
    void PostUpdate() override;
    void InspectorDisplay() override;


    [[nodiscard]] const GameObjectPtr& GetChild(glm::uint index) const { return children.at(index); }
    [[nodiscard]] const std::vector<GameObjectPtr>& GetChildren() const { return children; }
    [[nodiscard]] const Transform& GetTransform() const { return transform; }

    template <typename T>
    [[nodiscard]] std::shared_ptr<T> GetComponentOfType() const;

private:

    GameObjectPtr parent;
    
    Transform transform = Transform(this);

    std::vector<ComponentPtr> components;
    std::vector<GameObjectPtr> children;

};

#endif //GAMEOBJECT_H
