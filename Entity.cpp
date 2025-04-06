//  Entity.cpp
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

#include "Entity.hpp"
#include "Map.hpp"
#include <cmath>
#include <SDL.h>

Entity::Entity()
    : position(0), movement(0), velocity(0), acceleration(0, -9.81f, 0),
      speed(2.5f), jump_power(5.0f), width(0.8f), height(0.8f),
      texture_id(0), ai_type(NONE), collided_bottom(false), active(true), is_collectible(false) {}

void Entity::set_position(glm::vec3 p) { position = p; }
void Entity::set_movement(glm::vec3 m) { movement = m; }
void Entity::set_speed(float s)        { speed = s; }
void Entity::set_texture_id(GLuint t)  { texture_id = t; }
void Entity::set_ai_type(AIType t)     { ai_type = t; }
void Entity::set_width(float w)        { width = w; }
void Entity::set_height(float h)       { height = h; }
void Entity::set_is_collectible(bool value) { is_collectible = value; }

glm::vec3 Entity::get_position() const     { return position; }
glm::vec3 Entity::get_movement() const     { return movement; }
bool Entity::get_collided_bottom() const   { return collided_bottom; }
bool Entity::is_dead() const               { return !active; }
bool Entity::get_is_collectible() const    { return is_collectible; }

void Entity::move_left()  { movement.x = -1; }
void Entity::move_right() { movement.x = 1; }
void Entity::normalise_movement() { movement = glm::normalize(movement); }

void Entity::jump() {
    if (collided_bottom) velocity.y += jump_power;
}

void Entity::reset() {
    active = true;
    position = glm::vec3(5, 0, 0);  // Reset to level start
    velocity = glm::vec3(0);
}

void Entity::ai_activate(Entity* player) {
    switch (ai_type) {
        case WALKER: ai_walk(); break;
        case GUARD:  ai_guard(player); break;
        case FLYER:  ai_fly(); break;
        default: break;
    }
}

void Entity::ai_walk() {
    movement = glm::vec3(-1.0f, 0.0f, 0.0f);  // Constant left
}

void Entity::ai_guard(Entity* player) {
    float distance = glm::distance(position, player->get_position());
    if (distance < 3.0f) {
        movement.x = (player->get_position().x < position.x) ? -1 : 1;
    } else {
        movement.x = 0;
    }
}

void Entity::ai_fly() {
    // Sinusoidal vertical flying motion
    movement.y = sin(SDL_GetTicks() / 250.0f);
}

void Entity::update(float delta_time, Entity* player, std::vector<Entity*>& enemies, Map* map) {
    if (!active) return;

    if (ai_type != NONE) ai_activate(player);

    velocity.x = movement.x * speed;
    velocity += acceleration * delta_time;

    position.y += velocity.y * delta_time;
    check_collisions_y(map);

    position.x += velocity.x * delta_time;
    check_collisions_x(map);

    // Check collision with enemies or collectibles
    if (this == player) {
        for (Entity* entity : enemies) {
            if (!entity->active) continue;

            float xdist = fabs(position.x - entity->position.x) - ((width + entity->width) / 2.0f);
            float ydist = fabs(position.y - entity->position.y) - ((height + entity->height) / 2.0f);

            if (xdist < 0 && ydist < 0) {
                if (entity->get_is_collectible()) {
                    entity->active = false; // Collect the hat
                } else {
                    active = false; // Player dies
                    break;
                }
            }
        }
    }
}

void Entity::check_collisions_y(Map* map) {
    collided_bottom = false;
    int bottom = (int)floor(position.y - height / 2.0f);
    int left   = (int)floor(position.x - width / 2.0f);
    int right  = (int)floor(position.x + width / 2.0f);

    if (map->is_solid(left, bottom) || map->is_solid(right, bottom)) {
        collided_bottom = true;
        velocity.y = 0;
        position.y = bottom + 1 + height / 2.0f;
    }
}

void Entity::check_collisions_x(Map* map) {
    int top = (int)floor(position.y + height / 2.0f);
    int bottom = (int)floor(position.y - height / 2.0f);
    int side = (velocity.x > 0) ? (int)floor(position.x + width / 2.0f)
                                : (int)floor(position.x - width / 2.0f);

    for (int y = bottom; y <= top; y++) {
        if (map->is_solid(side, y)) {
            velocity.x = 0;
            break;
        }
    }
}

void Entity::render(ShaderProgram* shader) {
    if (!active) return;

    float vertices[] = {
        -width / 2, -height / 2,
         width / 2, -height / 2,
         width / 2,  height / 2,
        -width / 2, -height / 2,
         width / 2,  height / 2,
        -width / 2,  height / 2
    };

    float tex_coords[] = {
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 0, 0, 0
    };

    GLuint vao, vbo[2];
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(2, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, nullptr);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    shader->set_model_matrix(model);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDeleteBuffers(2, vbo);
    glDeleteVertexArrays(1, &vao);
}
