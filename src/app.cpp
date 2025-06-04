#include "app.hpp"
#include "glad/gl.h"
#include "window.hpp"
#include <GLFW/glfw3.h>

namespace kwnc
{
App::App()
    : window{WIDTH, HEIGHT, "Kevin Craft"},
      shader{"assets/shaders/frag.glsl", "assets/shaders/vert.glsl"},
      chunk{0, 0, 0}
{
        chunk.generate_mesh();
        glEnable(GL_DEPTH_TEST);
}
App::~App() {}

void App::run()
{
        while (!glfwWindowShouldClose(window.get_glfw_window())) {
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                if (glfwGetKey(window.get_glfw_window(), GLFW_KEY_ESCAPE) ==
                    GLFW_PRESS) {
                        glfwSetWindowShouldClose(window.get_glfw_window(), 1);
                }

                shader.use();
                chunk.vertex_array.draw();

                // RENDER
                glfwSwapBuffers(window.get_glfw_window());
                glfwWaitEvents();
        }
}
} // namespace kwnc
