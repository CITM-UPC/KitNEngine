//
// Created by Roger on 9/11/2024.
//

#ifndef GAME_H
#define GAME_H
#include "App.h"
#include <memory>

class Input;
class EntityManager;
class ResourceManager;

class Game;
using GamePtr = std::unique_ptr<Game>;

class Game final : private App {
public: // Methods

    Game();
    ~Game();

    bool Init() override;
    bool Start() override;
    bool Update() override;
    bool CleanUp() override;


public: // Member variables

    std::shared_ptr<Input> input;
    std::shared_ptr<EntityManager> entities;
    std::shared_ptr<ResourceManager> resources;
    
};

extern GamePtr game;

#endif //GAME_H
