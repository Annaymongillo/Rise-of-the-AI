//
//  Effects.cpp
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


#include "Effects.hpp"
#include <cmath>
#include <SDL.h>


Effects::Effects(glm::mat4 proj, glm::mat4 v)
    : projection(proj), view(v), effect_type(NONE), effect_time(0), effect_duration(0), view_offset(0.0f) {}

void Effects::start(EffectType type, float duration) {
    effect_type = type;
    effect_duration = duration;
    effect_time = 0.0f;
    view_offset = glm::vec3(0.0f);
}

void Effects::update(float delta_time) {
    if (effect_type == NONE) return;

    effect_time += delta_time;
    if (effect_time >= effect_duration) {
        effect_type = NONE;
        view_offset = glm::vec3(0.0f);
        view = glm::mat4(1.0f); // Reset view
        return;
    }

    switch (effect_type) {
        case SHAKE: {
            float shake_x = ((rand() % 100) / 100.0f - 0.5f) * shake_magnitude;
            float shake_y = ((rand() % 100) / 100.0f - 0.5f) * shake_magnitude;
            view_offset = glm::vec3(shake_x, shake_y, 0);
            break;
        }

        case SHRINK: {
            float scale = 1.0f - (effect_time / effect_duration);
            view = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f));
            break;
        }

        default:
            break;
    }
}

void Effects::render() {
}

glm::vec3 Effects::get_view_offset() const {
    return view_offset;
}
