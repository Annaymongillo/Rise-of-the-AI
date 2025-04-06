//  Level3.cpp
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

#include "Level3.hpp"
#include "Utility.hpp"
#include <SDL2_mixer/SDL_mixer.h>

#define LEVEL3_WIDTH  60
#define LEVEL3_HEIGHT 8

unsigned int LEVEL3_DATA[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

void Level3::initialise() {
    GLuint tile_texture = Utility::load_texture("Assets/graphics/vault_tiles.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, LEVEL3_DATA, tile_texture, 1.0f, 4, 1);

    // Player setup
    GLuint raccoon_texture_id = Utility::load_texture("Assets/graphics/raccoon_spritesheet.png");
    state.player = new Entity();
    state.player->set_position(glm::vec3(5, 0, 0));
    state.player->set_texture_id(raccoon_texture_id);
    state.player->set_speed(3.0f);
    state.player->set_animation(4, 1, new int[4]{0, 1, 2, 3}, 4, 0.25f);

    // FLYER AI (Bat)
    Entity* bat = new Entity();
    bat->set_position(glm::vec3(45, 2, 0));
    bat->set_ai_type(FLYER);
    bat->set_texture_id(Utility::load_texture("Assets/graphics/bat.png"));
    enemies.push_back(bat);

    // Collectible hats
    GLuint hat_texture = Utility::load_texture("Assets/graphics/hat.png");
    for (int i = 15; i <= 55; i += 10) {
        Entity* hat = new Entity();
        hat->set_position(glm::vec3(i, 1, 0));
        hat->set_texture_id(hat_texture);
        hat->set_width(0.6f);
        hat->set_height(0.6f);
        hats.push_back(hat);
    }

    // Audio
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    state.jump_sfx = Mix_LoadWAV("Assets/sfx/jump.wav");
    state.bgm = Mix_LoadMUS("Assets/music/heist_loop.ogg");
    Mix_PlayMusic(state.bgm, -1);
}

void Level3::update(float delta_time) {
    state.player->update(delta_time, state.player, enemies, state.map);
    for (Entity* enemy : enemies) {
        enemy->update(delta_time, state.player, enemies, state.map);
    }

    // Hat collection
    for (Entity* hat : hats) {
        if (hat->is_active() && glm::distance(state.player->get_position(), hat->get_position()) < 0.75f) {
            hat->deactivate();
        }
    }

    if (state.player->get_position().x > LEVEL3_WIDTH - 5) {
        state.next_scene_id = 5; // → WinScene
    }

    if (state.player->is_dead()) {
        state.lives--;
        if (state.lives <= 0) {
            state.next_scene_id = 4; // → GameOverScene
        } else {
            state.player->reset();
        }
    }
}

void Level3::render(ShaderProgram* shader) {
    state.map->render(shader);
    state.player->render(shader);
    for (Entity* enemy : enemies) {
        enemy->render(shader);
    }
    for (Entity* hat : hats) {
        if (hat->is_active()) hat->render(shader);
    }

    Utility::draw_text(shader, "Lives: " + std::to_string(state.lives), 0.3f, 0.02f,
                       glm::vec3(-4.5f, 3.0f, 0));
}
