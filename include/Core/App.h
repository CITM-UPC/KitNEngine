//
// Created by Roger on 9/11/2024.
//

#ifndef APP_H
#define APP_H
#include <memory>
#include <vector>

#include "Modules/Module.h"

class Input;
class EntityManager;

class App;
using AppPtr = std::unique_ptr<App>;

class App {
public: // Methods

    App();

    ~App();

    bool Init();

    bool Start();

    bool Update();

    bool CleanUp();

    bool AddModule(const ModulePtr& module);


public: // Member variables

    std::shared_ptr<Input> input;
    std::shared_ptr<EntityManager> entities;

private:
    std::vector<ModulePtr> modules;
    
};

extern AppPtr app;

#endif //APP_H
