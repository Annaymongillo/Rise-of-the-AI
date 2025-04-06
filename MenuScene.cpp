//
//  MenuScene.cpp
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

#include "MenuScene.hpp"
#include "Utility.hpp"

void MenuScene::initialise() {
    // No player or map in menu
    state.player = nullptr;
    state.map = nullptr;
    state.next_scene_id = -1;

    // Load the font texture for menu text
    state.font_texture_id = Utility::load_texture("fontsheet.png");
}

void MenuScene::update(float delta_time) {
}

void MenuScene::render(ShaderProgram* shader) {
    Utility::draw_text(shader, state.font_texture_id, "HAT HEIST", 0.6f, 0.05f, glm::vec3(-2.5f, 0.5f, 0));
    Utility::draw_text(shader, state.font_texture_id, "Press ENTER to start", 0.3f, 0.02f, glm::vec3(-3.0f, -1.0f, 0));
}
