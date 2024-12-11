//
// Created by Roger on 9/11/2024.
//

#ifndef APP_H
#define APP_H
#include "Modules/Module.h"
#include <memory>
#include <vector>


class App;
using AppPtr = std::unique_ptr<App>;

//Base class for program
class App {
public: // Methods

    App();

    virtual ~App();

    virtual bool Init() = 0;

    virtual bool Start();

    virtual bool Update();

    virtual bool CleanUp();

    bool AddModule(const ModulePtr& module);

    [[nodiscard]] ModulePtr GetModule(const std::string& name) const;

protected:
    std::vector<ModulePtr> modules;
    
};

#endif //APP_H
