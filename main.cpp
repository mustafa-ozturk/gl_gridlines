#include <GLFW/glfw3.h>
#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>

#include "gl_gridlines.h"

const unsigned int SCREEN_WIDTH = 500;
const unsigned int SCREEN_HEIGHT = 500;

using namespace gl;

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

    glbinding::initialize(glfwGetProcAddress);

    gl_gridlines gridlines(SCREEN_WIDTH, SCREEN_HEIGHT, 50, {1.0f, 0.5f, 0.2f, 0.1f});

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gridlines.draw_gridlines();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
