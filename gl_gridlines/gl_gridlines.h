#pragma once
#include <iostream>
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
using namespace gl;

class gl_gridlines
{
public:
    gl_gridlines(unsigned int screen_width, unsigned int screen_height);
    ~gl_gridlines();
    void draw_gridlines();
private:
    void create_gridline_data();
    void setup_gl_objects();
    void set_projection_view();

    unsigned int m_vbo, m_vao, m_ebo;
    std::vector<float> m_verticies;
    std::vector<unsigned int> m_indices;
    unsigned int m_screen_width;
    unsigned int m_screen_height;
    int m_grid_size = 50;
    int m_lines = 0;
    float m_line_colors[4] = {1.0f, 0.5f, 0.2f, 0.1f};

    unsigned int create_shader_program(const std::string& vertex_source, const std::string& fragment_source);
    unsigned int m_shader_program;
};
