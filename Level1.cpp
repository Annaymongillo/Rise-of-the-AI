//  Level1.cpp
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

#include "Level1.hpp"
#include "Utility.hpp"
#include <SDL2_mixer/SDL_mixer.h>

#define LEVEL1_WIDTH  60
#define LEVEL1_HEIGHT 8

unsigned int LEVEL1_DATA[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

void Level1::initialise() {
    GLuint tile_texture_id = Utility::load_texture("Assets/graphics/rooftops.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, LEVEL1_DATA, tile_texture_id, 1.0f, 4, 1);

    GLuint raccoon_texture_id = Utility::load_texture("Assets/graphics/raccoon_spritesheet.png");
    state.player = new Entity();
    state.player->set_position(glm::vec3(5, 0, 0));
    state.player->set_texture_id(raccoon_texture_id);
    state.player->set_speed(3.0f);
    state.player->set_animation(4, 1, new int[4]{0, 1, 2, 3}, 4, 0.25f);

    Entity* walker = new Entity();
    walker->set_position(glm::vec3(15, 0, 0));
    walker->set_ai_type(WALKER);
    walker->set_texture_id(Utility::load_texture("Assets/graphics/guard.png"));
    enemies.push_back(walker);

    // Hat collectible
    Entity* hat = new Entity();
    hat->set_position(glm::vec3(25, 0.5f, 0));
    hat->set_texture_id(Utility::load_texture("Assets/graphics/hat.png"));
    collectibles.push_back(hat);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    state.jump_sfx = Mix_LoadWAV("Assets/sfx/jump.wav");
    state.bgm = Mix_LoadMUS("Assets/music/heist_loop.ogg");
    Mix_PlayMusic(state.bgm, -1);
}

void Level1::update(float delta_time) {
    state.player->update(delta_time, state.player, enemies, state.map);
    for (Entity* enemy : enemies) enemy->update(delta_time, state.player, enemies, state.map);

    // Check for hat collection
    for (Entity* hat : collectibles) {
        if (!hat->is_dead() && glm::distance(hat->get_position(), state.player->get_position()) < 0.75f) {
            hat->deactivate();
            // Play sound or increment score here
        }
    }

    if (state.player->get_position().x > LEVEL1_WIDTH - 5) state.next_scene_id = 2;

    if (state.player->is_dead()) {
        state.lives--;
        if (state.lives <= 0) state.next_scene_id = 4;
        else state.player->reset();
    }
}

void Level1::render(ShaderProgram* shader) {
    state.map->render(shader);
    for (Entity* hat : collectibles) if (!hat->is_dead()) hat->render(shader);
    state.player->render(shader);
    for (Entity* enemy : enemies) enemy->render(shader);

    Utility::draw_text(shader, "Lives: " + std::to_string(state.lives), 0.3f, 0.02f, glm::vec3(-4.5f, 3.0f, 0));
}
