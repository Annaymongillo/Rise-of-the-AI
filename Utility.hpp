//
//  Utility.hpp
//  HatHeistProject
//


#ifndef Utility_hpp
#define Utility_hpp

#include <stdio.h>
#pragma once

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "ShaderProgram.h"

namespace Utility {
    GLuint load_texture(const char* filepath);
    void draw_text(ShaderProgram* program, GLuint font_texture_id,
                   std::string text, float size, float spacing,
                   glm::vec3 position);
}

#endif /* Utility_hpp */
