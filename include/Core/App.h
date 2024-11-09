//
// Created by Roger on 9/11/2024.
//

#ifndef APP_H
#define APP_H
#include <memory>
#include <vector>

#include "Modules/Input.h"
#include "Modules/Module.h"

class Input;

class App {
private:
    App();
public: // Methods

    static App& GetInstance();

    bool Init();

    bool Start();

    bool Update();

    bool CleanUp();


public: // Member variables

    std::shared_ptr<Input> input;


private:
    std::vector<ModulePtr> modules;
    
};



#endif //APP_H
