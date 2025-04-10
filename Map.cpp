//
//  Map.cpp
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

#include "Map.hpp"

Map::Map(int width, int height, unsigned int* level_data,
         GLuint texture_id, float tile_size,
         int tile_count_x, int tile_count_y)
    : m_width(width), m_height(height), m_level_data(level_data),
      m_texture_id(texture_id), m_tile_size(tile_size),
      m_tile_count_x(tile_count_x), m_tile_count_y(tile_count_y) {
    build();
}

void Map::build() {
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            int tile = m_level_data[y * m_width + x];
            if (tile == 0) continue;

            float u = (float)(tile % m_tile_count_x) / m_tile_count_x;
            float v = (float)(tile / m_tile_count_x) / m_tile_count_y;
            float tile_width = 1.0f / m_tile_count_x;
            float tile_height = 1.0f / m_tile_count_y;

            float x_offset = -(m_tile_size / 2);
            float y_offset = (m_tile_size / 2);

            float x_pos = x_offset + (m_tile_size * x);
            float y_pos = y_offset + (-m_tile_size * y);

            m_vertices.insert(m_vertices.end(), {
                x_pos, y_pos,
                x_pos, y_pos - m_tile_size,
                x_pos + m_tile_size, y_pos - m_tile_size,

                x_pos, y_pos,
                x_pos + m_tile_size, y_pos - m_tile_size,
                x_pos + m_tile_size, y_pos
            });

            m_texture_coordinates.insert(m_texture_coordinates.end(), {
                u, v,
                u, v + tile_height,
                u + tile_width, v + tile_height,

                u, v,
                u + tile_width, v + tile_height,
                u + tile_width, v
            });
        }
    }

    m_left_bound   = 0 - (m_tile_size / 2);
    m_right_bound  = (m_tile_size * m_width) - (m_tile_size / 2);
    m_top_bound    = 0 + (m_tile_size / 2);
    m_bottom_bound = -(m_tile_size * m_height) + (m_tile_size / 2);
}

void Map::render(ShaderProgram* program) {
    glm::mat4 model_matrix = glm::mat4(1.0f);
    program->set_model_matrix(model_matrix);
    glUseProgram(program->get_program_id());

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, m_vertices.data());
    glEnableVertexAttribArray(program->get_position_attribute());

    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, m_texture_coordinates.data());
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, (int)m_vertices.size() / 2);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

bool Map::is_solid(int tile_x, int tile_y) const {
    if (tile_x < 0 || tile_x >= m_width || tile_y < 0 || tile_y >= m_height) return false;
    return m_level_data[tile_y * m_width + tile_x] != 0;
}

bool Map::is_solid(glm::vec3 position, float* penetration_x, float* penetration_y) {
    if (penetration_x) *penetration_x = 0;
    if (penetration_y) *penetration_y = 0;

    if (position.x < m_left_bound || position.x > m_right_bound) return false;
    if (position.y > m_top_bound || position.y < m_bottom_bound) return false;

    int tile_x = floor((position.x + (m_tile_size / 2)) / m_tile_size);
    int tile_y = -(ceil(position.y - (m_tile_size / 2)) / m_tile_size);

    if (!is_solid(tile_x, tile_y)) return false;

    float tile_center_x = tile_x * m_tile_size;
    float tile_center_y = -tile_y * m_tile_size;

    if (penetration_x)
        *penetration_x = (m_tile_size / 2) - fabs(position.x - tile_center_x);
    if (penetration_y)
        *penetration_y = (m_tile_size / 2) - fabs(position.y - tile_center_y);

    return true;
}
