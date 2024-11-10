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

    bool Awake() override;
    bool Start() override;
    bool PreUpdate() override;
    bool Update() override;
    bool PostUpdate() override;
    bool InspectorDisplay() override;
    bool CleanUp() override;
    // TODO afegir Enable/Disable


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

    bool _active = true;

};

#endif //GAMEOBJECT_H
