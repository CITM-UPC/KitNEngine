//
// Created by Roger on 4/12/2024.
//

#ifndef INSPECTOR_H
#define INSPECTOR_H
#include "Module.h"


class Inspector final : public Module {
public:
    Inspector();
    ~Inspector() override;
    bool Init() override;
    bool Start() override;
    bool PreUpdate() override;
    bool Update() override;
    bool LateUpdate() override;
    bool EditorUI() override;
    bool CleanUp() override;
};



#endif //INSPECTOR_H
