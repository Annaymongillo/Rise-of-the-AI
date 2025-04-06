//
//  Map.hpp
//  HatHeistProject
//


#ifndef Map_hpp
#define Map_hpp

#include <stdio.h>
#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "ShaderProgram.h"

class Map {
public:
    Map(int width, int height, unsigned int* level_data,
        GLuint texture_id, float tile_size,
        int tile_count_x, int tile_count_y);

    void build();
    void render(ShaderProgram* program);
    bool is_solid(int tile_x, int tile_y) const;
    bool is_solid(glm::vec3 position, float* penetration_x = nullptr, float* penetration_y = nullptr);

private:
    int m_width;
    int m_height;

    GLuint m_texture_id;
    float m_tile_size;
    int m_tile_count_x;
    int m_tile_count_y;

    float m_left_bound;
    float m_right_bound;
    float m_top_bound;
    float m_bottom_bound;

    unsigned int* m_level_data;
    std::vector<float> m_vertices;
    std::vector<float> m_texture_coordinates;
};

#endif /* Map_hpp */
