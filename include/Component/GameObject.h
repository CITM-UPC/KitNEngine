//
// Created by Roger on 02/10/2024.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Transform.h"
#include "OpenEXR/IlmThreadProcessGroup.h"

class GameObject;

using GameObjectPtr = std::shared_ptr<GameObject>;

class GameObject : public Component
{
    public:
    GameObject();
    ~GameObject();

    void InspectorDisplay() override;
    

    [[nodiscard]] const GameObjectPtr& GetChild(glm::uint index) const { return children.at(index); }
    [[nodiscard]] const std::vector<GameObjectPtr>& GetChildren() const { return children; }
    [[nodiscard]] const Transform& GetTransform() const { return transform; }

    template <typename T>
    [[nodiscard]] std::shared_ptr<T> GetComponentOfType() const;

private:
    
    Transform transform;

    std::vector<ComponentPtr> components;
    std::vector<GameObjectPtr> children;

};

#endif //GAMEOBJECT_H
