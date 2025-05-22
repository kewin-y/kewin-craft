#include "app.hpp"
#include "glad/gl.h"
#include "window.hpp"
#include <GLFW/glfw3.h>

namespace kwnc
{
App::App() : window{WIDTH, HEIGHT, "Kevin Craft"} {}
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

                // RENDER
                glfwSwapBuffers(window.get_glfw_window());
                glfwWaitEvents();
        }
}
} // namespace kwnc
