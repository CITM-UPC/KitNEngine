//
// Created by Roger on 11/12/2024.
//

#ifndef EDITOR_H
#define EDITOR_H
#include "Game/Core/App.h"

class GameObject;
class Camera;
class Inspector;

class Editor;
using EditorPtr = std::shared_ptr<Editor>;

class Editor : private App {
public: // Methods
    
    Editor();
    ~Editor();
    
    bool Init() override;
    bool Start() override;
    bool Update() override;
    bool CleanUp() override;


public: // Member variables

    std::shared_ptr<Inspector> inspector;

    std::shared_ptr<GameObject> editorObject;
    std::shared_ptr<Camera> editorCamera;
    
};

extern EditorPtr editor;

#endif //EDITOR_H
