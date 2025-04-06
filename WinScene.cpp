//
//  WinScene.cpp
//  HatHeistProject
//
/**
* Author: Annay Mongillo
* Assignment: Rise of the AI
* Date due: 2025-04-05, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "WinScene.hpp"
#include "Utility.hpp"
#include <SDL2_mixer/SDL_mixer.h>

void WinScene::initialise() {
    state.next_scene_id = -1;

    // Load the font texture for rendering text
    state.font_texture_id = Utility::load_texture("fontsheet.png");

    // Load and play the victory sound
    state.jump_sfx = Mix_LoadWAV("Assets/sfx/victory.wav");
    Mix_PlayChannel(-1, state.jump_sfx, 0);
}

void WinScene::update(float delta_time) {
}

void WinScene::render(ShaderProgram* shader) {
    Utility::draw_text(shader, state.font_texture_id, "YOU STOLE ALL THE HATS!", 0.5f, 0.03f, glm::vec3(-3.5f, 0.5f, 0));
    Utility::draw_text(shader, state.font_texture_id, "Press ENTER to replay", 0.3f, 0.02f, glm::vec3(-3.0f, -1.0f, 0));
}
