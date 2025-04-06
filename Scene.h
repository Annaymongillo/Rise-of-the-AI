//
//  Scene.h
//  HatHeistProject
//


#ifndef Scene_h
#define Scene_h

#pragma once

#include "Map.hpp"
#include "Entity.hpp"
#include <vector>
#include <SDL2_mixer/SDL_mixer.h>

// Forward declaration
class ShaderProgram;

struct SceneState {
    Entity* player;
    std::vector<Entity*> enemies;
    Map* map;

    GLuint font_texture_id;
    int next_scene_id;

    Mix_Chunk* jump_sfx;
};

class Scene {
public:
    virtual ~Scene() {}

    virtual void initialise() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram* program) = 0;

    SceneState& get_state() { return state; }

protected:
    SceneState state;
};
#endif /* Scene_h */
