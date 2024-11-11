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
    
    GameObject(GameObjectPtr go);
    ~GameObject();

    bool Awake() override;
    bool Start() override;
    bool PreUpdate() override;
    bool Update() override;
    bool PostUpdate() override;
    bool InspectorDisplay() override;
    bool CleanUp() override;
    


    [[nodiscard]] GameObjectPtr& GetChild(glm::uint index) { return children.at(index); }
    [[nodiscard]] std::vector<GameObjectPtr>& GetChildren() { return children; }
    [[nodiscard]] const TransformPtr GetTransform() { return transform; }

    template <typename T>
    [[nodiscard]] std::shared_ptr<T> GetComponentOfType() const;
    GameObjectPtr AddChild(GameObjectPtr g);

private:

    GameObjectPtr parent;
    
    TransformPtr transform = std::make_shared<Transform>(this);

    std::vector<ComponentPtr> components;
    std::vector<GameObjectPtr> children;

    bool _active = true;

};

#endif //GAMEOBJECT_H
