//
// Created by Roger on 02/10/2024.
//

#ifndef IGINTERFACE_H
#define IGINTERFACE_H
#include "Modules/Module.h"


// Main module for imGui interface management (?)
class IGInterface : Module {
public:
    IGInterface();
    ~IGInterface();

    bool StartFrame();
    bool EndFrame();
};



#endif //IGINTERFACE_H
