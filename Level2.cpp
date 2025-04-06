//  Level2.cpp
//  HatHeistProject
/**
* Author: Annay Mongillo
* Assignment: Rise of the AI
* Date due: 2025-04-05, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Level2.hpp"
#include "Utility.hpp"
#include <SDL2_mixer/SDL_mixer.h>

#define LEVEL2_WIDTH  60
#define LEVEL2_HEIGHT 8

unsigned int LEVEL2_DATA[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

void Level2::initialise() {
    // Load tile map
    GLuint tile_texture = Utility::load_texture("Assets/graphics/museum_tiles.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, LEVEL2_DATA, tile_texture, 1.0f, 4, 1);

    // Player setup
    GLuint raccoon_texture = Utility::load_texture("Assets/graphics/raccoon_spritesheet.png");
    state.player = new Entity();
    state.player->set_position(glm::vec3(5, 0, 0));
    state.player->set_texture_id(raccoon_texture);
    state.player->set_speed(3.0f);
    state.player->set_animation(4, 1, new int[4]{0, 1, 2, 3}, 4, 0.25f);

    // GUARD AI 
    Entity* snail = new Entity();
    snail->set_position(glm::vec3(30, 0, 0));
    snail->set_ai_type(GUARD);
    snail->set_texture_id(Utility::load_texture("Assets/graphics/snail.png"));
    enemies.push_back(snail);

    // Hat collectibles
    GLuint hat_texture = Utility::load_texture("Assets/graphics/hat.png");
    for (int i = 0; i < 3; i++) {
        Entity* hat = new Entity();
        hat->set_position(glm::vec3(10 + i * 8, 1, 0));
        hat->set_texture_id(hat_texture);
        collectibles.push_back(hat);
    }

    // Load audio
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    state.jump_sfx = Mix_LoadWAV("Assets/sfx/jump.wav");
    state.bgm = Mix_LoadMUS("Assets/music/heist_loop.ogg");
    Mix_PlayMusic(state.bgm, -1);
}

void Level2::update(float delta_time) {
    state.player->update(delta_time, state.player, enemies, state.map);
    for (Entity* enemy : enemies) {
        enemy->update(delta_time, state.player, enemies, state.map);
    }

    // Hat collection logic
    for (Entity* hat : collectibles) {
        if (!hat->is_dead()) {
            float distance = glm::distance(state.player->get_position(), hat->get_position());
            if (distance < 0.7f) {
                hat->deactivate();
            }
        }
    }

    // Level complete
    if (state.player->get_position().x > LEVEL2_WIDTH - 5) {
        state.next_scene_id = 3; // → Level3
    }

    // Death
    if (state.player->is_dead()) {
        state.lives--;
        if (state.lives <= 0) {
            state.next_scene_id = 4; // → Game Over
        } else {
            state.player->reset();
        }
    }
}

void Level2::render(ShaderProgram* shader) {
    state.map->render(shader);
    state.player->render(shader);
    for (Entity* enemy : enemies) {
        enemy->render(shader);
    }
    for (Entity* hat : collectibles) {
        if (!hat->is_dead()) hat->render(shader);
    }

    Utility::draw_text(shader, "Lives: " + std::to_string(state.lives), 0.3f, 0.02f,
                       glm::vec3(-4.5f, 3.0f, 0));
}
