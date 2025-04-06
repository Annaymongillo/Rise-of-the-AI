//  Entity.hpp
//  HatHeistProject
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#pragma once

#include "ShaderProgram.h"
#include "glm/glm.hpp"
#include <vector>

class Map;

enum AIType { NONE, WALKER, GUARD, FLYER };

class Entity {
public:
    Entity();

    void update(float delta_time, Entity* player, std::vector<Entity*>& enemies, Map* map);
    void render(ShaderProgram* shader);

    void set_position(glm::vec3 p);
    void set_movement(glm::vec3 m);
    void set_speed(float s);
    void set_texture_id(GLuint t);
    void set_ai_type(AIType t);
    void set_width(float w);
    void set_height(float h);
    void set_collectible(bool flag);

    glm::vec3 get_position() const;
    glm::vec3 get_movement() const;
    bool get_collided_bottom() const;
    bool is_dead() const;
    bool is_collectible_entity() const;

    void jump();
    void move_left();
    void move_right();
    void normalise_movement();
    void reset();

private:
    // Movement & AI
    void ai_activate(Entity* player);
    void ai_walk();
    void ai_guard(Entity* player);
    void ai_fly();

    // Collision
    void check_collisions_y(Map* map);
    void check_collisions_x(Map* map);

    // State
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    float speed;
    float jump_power;
    float width, height;

    GLuint texture_id;
    AIType ai_type;

    bool collided_bottom;
    bool active;
    bool is_collectible; // NEW
};

#endif /* Entity_hpp */

