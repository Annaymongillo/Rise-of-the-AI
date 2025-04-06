//
//  Utility.cpp
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

#define STB_IMAGE_IMPLEMENTATION
#define FONTBANK_SIZE 16
#define LOG(x) std::cout << x << "\n"

#include "Utility.hpp"
#include <SDL_image.h>
#include "stb_image.h"
#include <iostream>
#include <cassert>

GLuint Utility::load_texture(const char* filepath) {
    int width, height, channels;
    unsigned char* image = stbi_load(filepath, &width, &height, &channels, STBI_rgb_alpha);

    if (!image) {
        LOG("Failed to load texture: " << filepath);
        assert(false);
    }

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(image);
    return texture_id;
}

void Utility::draw_text(ShaderProgram* program, GLuint font_texture_id,
                        std::string text, float size, float spacing,
                        glm::vec3 position) {
    float width = 1.0f / FONTBANK_SIZE;
    float height = 1.0f / FONTBANK_SIZE;

    std::vector<float> vertices;
    std::vector<float> tex_coords;

    for (int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (size + spacing) * i;

        float u = (float)(index % FONTBANK_SIZE) / FONTBANK_SIZE;
        float v = (float)(index / FONTBANK_SIZE) / FONTBANK_SIZE;

        vertices.insert(vertices.end(), {
            offset + (-0.5f * size),  0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size),  0.5f * size,

            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size),  0.5f * size,
            offset + (-0.5f * size), -0.5f * size
        });

        tex_coords.insert(tex_coords.end(), {
            u, v,
            u, v + height,
            u + width, v,

            u + width, v + height,
            u + width, v,
            u, v + height
        });
    }

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    program->set_model_matrix(model);
    glUseProgram(program->get_program_id());

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->get_position_attribute());

    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords.data());
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glBindTexture(GL_TEXTURE_2D, font_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}
