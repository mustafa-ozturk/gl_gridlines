#include <iostream>

#include <GLFW/glfw3.h>
#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

using namespace gl;

const unsigned int SCREEN_WIDTH = 500;
const unsigned int SCREEN_HEIGHT = 500;

#include "gl_gridlines.h"

const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 pos;
    layout (location = 1) in vec4 color;

    out vec4 vertexColor;

    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * vec4(pos.xyz, 1.0);
        vertexColor = color;
    }
)";

const char *fragmentShaderSource = R"(
    #version 330 core

    in vec4 vertexColor;

    out vec4 FragColor;

    void main()
    {
        FragColor = vertexColor;
    }
)";

int main()
{
    // glfw
    // ------------------------
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gl_gridlines", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // init glbiding
    // --------------
    glbinding::initialize(glfwGetProcAddress);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<float> vertices;

    const int GRID_SIZE = 50;
    int lines = 0;
    float line_colors[4] = {1.0f, 0.5f, 0.2f, 0.1f};

    // vertical lines
    for (int i = GRID_SIZE; i < SCREEN_HEIGHT; i += GRID_SIZE)
    {
        // first point of line
        vertices.push_back(0); // x
        vertices.push_back(i); // y
        vertices.push_back(1); // z

        // color
        vertices.push_back(line_colors[0]); // r
        vertices.push_back(line_colors[1]); // g
        vertices.push_back(line_colors[2]); // b
        vertices.push_back(line_colors[3]); // a

        // second point of line
        vertices.push_back(SCREEN_WIDTH); // x
        vertices.push_back(i); // y
        vertices.push_back(1); // z

        // color
        vertices.push_back(line_colors[0]); // r
        vertices.push_back(line_colors[1]); // g
        vertices.push_back(line_colors[2]); // b
        vertices.push_back(line_colors[3]); // a

        lines++;
    }

    // horizontal lines
    for (int i = GRID_SIZE; i < SCREEN_WIDTH; i += GRID_SIZE)
    {
        // first point of line
        vertices.push_back(i); // x
        vertices.push_back(0); // y
        vertices.push_back(1); // z

        // color
        vertices.push_back(line_colors[0]); // r
        vertices.push_back(line_colors[1]); // g
        vertices.push_back(line_colors[2]); // b
        vertices.push_back(line_colors[3]); // a

        // second point of line
        vertices.push_back(i); // x
        vertices.push_back(SCREEN_HEIGHT); // y
        vertices.push_back(1); // z

        // color
        vertices.push_back(line_colors[0]); // r
        vertices.push_back(line_colors[1]); // g
        vertices.push_back(line_colors[2]); // b
        vertices.push_back(line_colors[3]); // a

        lines++;
    }

    // horizontal center line
    vertices.push_back(SCREEN_WIDTH / 2); // x
    vertices.push_back(0); // y
    vertices.push_back(1); // z

    // color
    vertices.push_back(line_colors[0]); // r
    vertices.push_back(line_colors[1]); // g
    vertices.push_back(line_colors[2]); // b
    vertices.push_back(line_colors[0]); // a

    vertices.push_back(SCREEN_WIDTH / 2); // x
    vertices.push_back(SCREEN_HEIGHT); // y
    vertices.push_back(1); // z

    // color
    vertices.push_back(line_colors[0]); // r
    vertices.push_back(line_colors[1]); // g
    vertices.push_back(line_colors[2]); // b
    vertices.push_back(line_colors[0]); // a
    lines++;

    // vertical center line
    vertices.push_back(0); // x
    vertices.push_back(SCREEN_HEIGHT / 2); // y
    vertices.push_back(1); // z

    // color
    vertices.push_back(line_colors[0]); // r
    vertices.push_back(line_colors[1]); // g
    vertices.push_back(line_colors[2]); // b
    vertices.push_back(line_colors[0]); // a

    vertices.push_back(SCREEN_WIDTH); // x
    vertices.push_back(SCREEN_HEIGHT / 2); // y
    vertices.push_back(1); // z

    // color
    vertices.push_back(line_colors[0]); // r
    vertices.push_back(line_colors[1]); // g
    vertices.push_back(line_colors[2]); // b
    vertices.push_back(line_colors[0]); // a
    lines++;


    std::vector<unsigned int> indices;
    for (int i = 0; i < lines * 2; i++)
    {
        indices.push_back(i);
    }

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // projection
    // -----------
    glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT);
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    gl_gridlines gridlines(SCREEN_WIDTH, SCREEN_HEIGHT);

    // render loop
    // --------------------------
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gridlines.draw_gridlines();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
