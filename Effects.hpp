//
//  Effects.hpp
//  HatHeistProject


#ifndef Effects_hpp
#define Effects_hpp

#include <stdio.h>
#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

enum EffectType {
    NONE,
    FADEIN,
    FADEOUT,
    SHAKE,
    SHRINK
};

class Effects {
public:
    Effects(glm::mat4 projection, glm::mat4 view);

    void start(EffectType type, float duration);
    void update(float delta_time);
    void render();
    
    glm::vec3 get_view_offset() const;
    glm::mat4 get_view_matrix() const { return view; } // 🟢 Useful if needed externally

private:
    EffectType effect_type;
    float effect_time;
    float effect_duration;

    glm::vec3 view_offset;
    glm::mat4 projection;
    glm::mat4 view;

    float shake_magnitude = 0.1f;
};

#endif /* Effects_hpp */
