//
//  Game.hpp
//  HatHeistProject


#ifndef Game_hpp
#define Game_hpp

#pragma once

#include <stdio.h>
#include <SDL.h>
#include "ShaderProgram.h"
#include "Effects.hpp"
#include "Scene.h"

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void initialise();
    void process_input();
    void update();
    void render();
    void shutdown();
    void switch_to_scene(Scene* scene);

    SDL_Window* window;
    ShaderProgram shader;
    Effects* effects;
    Scene* scenes[6];       
    Scene* current_scene;

    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;

    float prev_ticks;
    float accumulator;
    bool running;
};

#endif /* Game_hpp */
